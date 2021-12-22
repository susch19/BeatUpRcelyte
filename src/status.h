#include <string.h>

static uint32_t status_gen(char *buf, const char *code, const char *mime, const char *resp) {
	return sprintf(buf,
		"HTTP/1.1 %s\r\n"
		"Connection: close\r\n"
		"Content-Length: %zd\r\n"
		"X-Frame-Options: DENY\r\n"
		"X-Content-Type-Options: nosniff\r\n"
		"Content-Type: %s; charset=utf-8\r\n"
		"X-DNS-Prefetch-Control: off\r\n"
		"X-Robots-Tag: noindex\r\n"
		"\r\n%s", code, strlen(resp), mime, resp);
}

static uint32_t status_resp(const char *source, const char *path, char *buf, uint32_t buf_len) {
	char rq[4096];
	uint32_t rq_len = sprintf(rq, "GET %s", path);
	if(buf_len < rq_len || memcmp(buf, rq, rq_len))
		return 0;
	fprintf(stderr, "[%s] %.*s\n", source, (int32_t)((char*)memchr(&buf[4], ' ', buf_len) - buf), buf);
	buf += rq_len, buf_len -= rq_len;
	if(buf_len && *buf == '/')
		++buf, --buf_len;
	if(buf_len && *buf == ' ')
		return status_gen(buf, "200 OK", "application/json", "{\"minimumAppVersion\":\"1.16.4\",\"status\":0,\"maintenanceStartTime\":0,\"maintenanceEndTime\":0,\"userMessage\":{\"localizedMessages\":[{\"language\":\"en\",\"message\":\"Test message from server\"}]}}");
	else if(buf_len > 17 && memcmp(buf, "mp_override.json ", 17) == 0)
		return status_gen(buf, "200 OK", "application/json", "{\"quickPlayAvailablePacksOverride\":{\"predefinedPackIds\":[{\"order\":0,\"packId\":\"ALL_LEVEL_PACKS\"},{\"order\":1,\"packId\":\"BUILT_IN_LEVEL_PACKS\"}],\"localizedCustomPacks\":[{\"serializedName\":\"customlevels\",\"order\":2,\"localizedNames\":[{\"language\":0,\"packName\":\"Custom\"}],\"packIds\":[\"custom_levelpack_CustomLevels\"]}]}}");
	else if(buf_len > 11 && memcmp(buf, "robots.txt ", 11) == 0)
		return status_gen(buf, "200 OK", "text/plain", "User-agent: *\nDisallow: /\n");
	else
		return status_gen(buf, "404 Not Found", "text/html", "<html><body>404 not found</body></html>");
}