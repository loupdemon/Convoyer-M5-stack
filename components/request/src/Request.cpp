#include "Request.h"

String Request::requestGet(String url, String token) {
    return Request::requestGeneric(url, HTTP_GET, token);
}

String Request::requestPost(String url, String data, String token) {
    return Request::requestGeneric(url, HTTP_POST, token, data);
}

String Request::requestPut(String url, String data, String token) {
    return Request::requestGeneric(url, HTTP_PUT, token, data);
}

String Request::requestGeneric(String url, enum HTTP_METHOD method, String token, String data) {
    // wait for Wi-Fi connection
    if ((WiFi.status() == WL_CONNECTED)) {
        WiFiClient client;
        HTTPClient http;

        M5.Lcd.print("[HTTP] begin...\n");

        // configure tagged server and url
        String uri = API_URL + url;

        http.begin(client, uri); //HTTP
        http.addHeader("Content-Type", "application/json");

        if (token.length() > 0) {
            const String HEADER = String(DOLAPIKEY);
            http.addHeader(HEADER, token);
        }

        // start connection and send HTTP header and body
        int httpCode = -1;
        switch(method) {
            case HTTP_GET:
                httpCode = http.GET();
                break;
            case HTTP_POST:
                httpCode = http.POST(data);
                break;
            case HTTP_PUT:
                httpCode = http.PUT(data);
                break;
            default:
                break;
        }

        // httpCode will be negative on error
        if (httpCode > 0) {
            // file found at server
            if (httpCode == HTTP_CODE_OK) {
                const String& payload = http.getString();
                return payload;
            }
        } else {
            const String& payload = http.getString();
            M5.Lcd.printf("[HTTP] GET... failed, error: %s\n", payload.c_str());
        }

        http.end();
    }

    return "";
}