#include "email.h"

char payload_text[MSG_SIZE];

struct upload_status
{
    int lines_read;
};

static size_t payload_source(char *ptr, size_t size, size_t nmemb, void *userp)
{
    struct upload_status *upload_ctx = (struct upload_status *)userp;
    const char *data;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1))
    {
        return 0;
    }

    if (upload_ctx->lines_read == 0)
    {

        data = payload_text;

        size_t len = strlen(data);
        memcpy(ptr, data, len);
        upload_ctx->lines_read++;

        return len;
    }
    return 0;
}

int send_email(char *message, struct recipient_list *recipients, struct Email *email)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    int rc = 0;
    CURL *curl;
    struct curl_slist *curl_recipients = NULL;
    struct upload_status upload_ctx = {0};
    struct recipient_list *current_recipient = recipients;

    snprintf(payload_text, 10000, "From: Router <%s>\r\nSubject: Event\r\n\r\n%s\r\n", email->sender_email, message);
    printf(payload_text);

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "smtps");
        curl_easy_setopt(curl, CURLOPT_URL, email->smtp_ip);
        curl_easy_setopt(curl, CURLOPT_PORT, email->smtp_port);
        curl_easy_setopt(curl, CURLOPT_USERNAME, email->username);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, email->password);
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, email->sender_email);

        while (current_recipient != NULL)
        {
            curl_recipients = curl_slist_append(curl_recipients, current_recipient->email);
            current_recipient = current_recipient->next;
        }
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, curl_recipients);

        rc = (int)curl_easy_perform(curl);
        curl_slist_free_all(curl_recipients);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return rc;
}