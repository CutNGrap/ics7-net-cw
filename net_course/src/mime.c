#include "mime.h"

#include <strings.h>

const char *detect_mime_type(char *filename) {
    char *ext = strrchr(filename, '/');
    if (ext == NULL) {
        ext = filename;
    }

    ext = strrchr(filename, '.');
    if (ext == NULL) {
        return "text/html";
    }

    ext++;
    if (strcasecmp(ext, "jpeg") == 0 || strcasecmp(ext, "jpg") == 0) {
        return "image/jpeg";
    }
    if (strcasecmp(ext, "gif") == 0) {
        return "image/gif";
    }
    if (strcasecmp(ext, "png") == 0) {
        return "image/png";
    }
    if (strcasecmp(ext, "js") == 0) {
        return "application/javascript";
    }
    if (strcasecmp(ext, "json") == 0) {
        return "application/json";
    }
    if (strcasecmp(ext, "css") == 0) {
        return "text/css";
    }
    if (strcasecmp(ext, "txt") == 0) {
        return "text/plain";
    }
    if (strcasecmp(ext, "pdf") == 0) {
        return "application/pdf";
    }
    if (strcasecmp(ext, "swf") == 0) {
        return "application/x-shockwave-flash";
    }

    return "text/unknown";
}
