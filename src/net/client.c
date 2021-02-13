#include "net.h"

SOCKET init_client(char *ip, char *port)
{
    int status;

    // Build server address
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *server_addr;
    status = getaddrinfo(ip, port, &hints, &server_addr);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo failed: %d\n", GETSOCKETERRNO());
        return -1;
    }
    char address_buffer[100]; // Print addr
    char service_buffer[100];
    getnameinfo(server_addr->ai_addr, server_addr->ai_addrlen,
        address_buffer, sizeof(address_buffer),
        service_buffer, sizeof(service_buffer),
        NI_NUMERICHOST);
    printf("%s %s\n", address_buffer, service_buffer);

    // Connect
    SOCKET server_sd;
    server_sd = socket(server_addr->ai_family, server_addr->ai_socktype,
        server_addr->ai_protocol);
    if (!ISVALIDSOCKET(server_sd)) {
        fprintf(stderr, "socket failed: %d\n", GETSOCKETERRNO());
        return -1;
    }
    status = connect(server_sd,
        server_addr->ai_addr, server_addr->ai_addrlen);
    if (status != 0) {
        fprintf(stderr, "connect failed: %d\n", GETSOCKETERRNO());
        freeaddrinfo(server_addr);
        return -1;
    }
    freeaddrinfo(server_addr);

    return server_sd;
}

// Pack and send a request, return the server's response
static struct tfs_res client_exchange(
    SOCKET server, struct tfs_req req, enum res_type target)
{
    uint8_t *packed;
    size_t packed_req_size = pack_req(&packed, req);

    // Send request
    int bytes_sent = send(server, packed, packed_req_size, 0);
    if (bytes_sent != packed_req_size) {
        fprintf(stderr, "send failed: did not send all req bytes\n");
        print_req(req, 1);
        free(packed);
        struct tfs_res eres = { .type = RES_NULL };
        return eres;
    }
    free(packed);

    // Read response
    uint8_t *raw_res = calloc(MAX_RES_LEN, 1);
    int recv_reslen = recv(server, raw_res, MAX_RES_LEN, 0);
    printf("received %d bytes.\n", recv_reslen);
    struct tfs_res res = unpack_res(raw_res);
    print_res(res, 0);

    // Check for errors
    if (res.type == RES_ERROR) {
        printf("%s\n", (char *) res.body);
        free(packed);
        free(raw_res);
        struct tfs_res eres = { .type = RES_NULL };
        return eres;
    }
    if (res.type != target) {
        printf("unimplemented response code for REQ%d: RES%d\n",
            (int)target, (int)res.type);
        free(packed);
        free(raw_res);
        struct tfs_res eres = { .type = RES_NULL };
        return eres;
    }
    free(raw_res);
    return res;
}

struct fs *client_get_fs(SOCKET server, uint8_t tfsid[])
{
    // Construct request
    struct tfs_req req = { .type = REQ_GET_FS, .body_len = 0 };
    memcpy(req.fsid, tfsid, FSID_LEN);
    struct tfs_res res = client_exchange(server, req, RES_FS);

    struct fs *dfs = deserialize_fs(res.body, res.body_len);
    return dfs;
}

struct file client_get_file(SOCKET server, uint8_t tfsid[], char *filename)
{
    // Make req
    size_t filename_len = strlen(filename);
    printf("FILENAME IS %ld\n", filename_len);
    if (filename_len >= FILENAME_SIZE) {
        printf("filename '%ld' too large\n", filename_len);
        struct file f = {};
        return f;
    }
    struct tfs_req req = {
        .type = REQ_GET_FILE,
        .body_len = filename_len+1 // +1 for the null term
    };
    memcpy(req.body, filename, filename_len);
    print_req(req, 1);

    uint8_t *packed;
    size_t packed_req_size = pack_req(&packed, req);

    // Send request
    int bytes_sent = send(server, packed, packed_req_size, 0);
    if (bytes_sent != packed_req_size) {
        fprintf(stderr, "send failed: did not send all req bytes\n");
        print_req(req, 1);
        free(packed);
        struct file f = {};
        return f;
    }
    free(packed);

    // Read response
    uint8_t *raw_res = calloc(MAX_RES_LEN, 1);
    int recv_reslen = recv(server, raw_res, MAX_RES_LEN, 0);
    printf("received %d bytes.\n", recv_reslen);
    struct tfs_res res = unpack_res(raw_res);
    print_res(res, 1);

    if (res.type == RES_ERROR) {
        printf("%s\n", (char *) res.body);
        free(packed);
        free(raw_res);
        struct file f = {};
        return f;
    }
    if (res.type != RES_FILE) {
        printf("unimplemented response code for REQ_GET_FILE: %d\n",
            (int) res.type);
        free(packed);
        free(raw_res);
        struct file f = {};
        return f;
    }

    struct file file = { .s = res.body_len };
    memcpy(file.name, filename, filename_len);
    file.bytes = malloc(res.body_len);
    memcpy(file.bytes, res.body, res.body_len);
    print_file(file, ASCII);
    free(raw_res);
    free(packed);

    return file;
}
