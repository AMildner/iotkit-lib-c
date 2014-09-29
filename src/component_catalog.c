/*
 * Authorization API module to communicate with IoT Cloud via REST APIs
 * Copyright (c) 2014, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 */

/**
 * @file authorization.c
 * @brief Implementation of Authorization API
 *
 * Provides features for communication with IoT Cloud server
 */

#include "iotkit.h"

char * listAllComponentCatalogs() {
    struct curl_slist *headers = NULL;
    char *url;
    char *response = NULL;

    appendHttpHeader(&headers, HEADER_AUTHORIZATION, getDeviceAuthorizationToken());

    if(prepareUrl(&url, configurations.base_url, configurations.list_components)) {

        doHttpGet(url, headers, &response);

        return response;
    }

    return NULL;
}

char * getComponentCatalogDetails() {
    struct curl_slist *headers = NULL;
    char *url;
    char *response = NULL;

    appendHttpHeader(&headers, HEADER_AUTHORIZATION, getDeviceAuthorizationToken());

    if(prepareUrl(&url, configurations.base_url, configurations.get_component_details)) {

        doHttpGet(url, headers, &response);

        return response;
    }

    return NULL;
}

char *createAnComponentCatalog(char *cmp_name, char *cmp_version, char *cmp_type, char *cmp_datatype, \
        char *cmp_format, bool isMinPresent, double cmp_minvalue, bool isMaxPresent, double cmp_maxvalue, \
        char *cmp_unit, char *cmp_display, char *cmp_command, char *cmd_name, char *cmd_value) {

    struct curl_slist *headers = NULL;
    char *url;
    char body[BODY_SIZE_MIN];
    char *response = NULL;

    if(cmp_name == NULL || cmp_version == NULL || \
            cmp_type == NULL || cmp_datatype == NULL || \
            cmp_format == NULL || cmp_unit == NULL || \
            cmp_display == NULL) {
        printf("Mandatory field missing to create component catalog\n");

        return NULL;
    }

    if(prepareUrl(&url, configurations.base_url, configurations.create_an_cmp_catalog)) {
        appendHttpHeader(&headers, HEADER_CONTENT_TYPE_NAME, HEADER_CONTENT_TYPE_JSON);
        appendHttpHeader(&headers, HEADER_AUTHORIZATION, getConfigAuthorizationToken());

        strcpy(body, "{");

        if(cmp_name != NULL) {
            strcat(body, "\"dimension\":\"");
            strcat(body, cmp_name);
            strcat(body, "\"");
        }

        if(cmp_version != NULL) {
            strcat(body, ",\"version\":\"");
            strcat(body, cmp_version);
            strcat(body, "\"");
        }

        if(cmp_type != NULL) {
            strcat(body, ",\"type\":\"");
            strcat(body, cmp_type);
            strcat(body, "\"");
        }

        if(cmp_datatype != NULL) {
            strcat(body, ",\"dataType\":\"");
            strcat(body, cmp_datatype);
            strcat(body, "\"");
        }

        if(cmp_format != NULL) {
            strcat(body, ",\"format\":\"");
            strcat(body, cmp_format);
            strcat(body, "\"");
        }

        char value[BODY_SIZE_MIN];
        if(isMinPresent == true) {
            sprintf(value, "%f", cmp_minvalue);
            strcat(body, ",\"min\":");
            strcat(body, value);
        }

        if(isMaxPresent == true) {
            sprintf(value, "%f", cmp_maxvalue);
            strcat(body, ",\"max\":");
            strcat(body, value);
        }

        if(cmp_unit != NULL) {
            strcat(body, ",\"measureunit\":\"");
            strcat(body, cmp_unit);
            strcat(body, "\"");
        }

        if(cmp_display != NULL) {
            strcat(body, ",\"display\":\"");
            strcat(body, cmp_display);
            strcat(body, "\"");
        }

        if(cmp_command != NULL) {
            strcat(body, ",\"command\":{\"parameters\":[");

            // TODO: should support multiple cmd_name & cmd_values
            if(cmd_name != NULL) {
                strcat(body, "{\"name\":\"");
                strcat(body, cmd_name);
                strcat(body, "\",\"values\":\"");
                strcat(body, cmd_value);
                strcat(body, "\"}");
            }

            strcat(body, "],\"commandString\":\"");
            strcat(body, cmp_command);
            strcat(body, "\"}");
        }



        strcat(body, "}");

        #if DEBUG
            printf("Prepared BODY is %s\n", body);
        #endif

        doHttpPost(url, headers, body, &response);

        return response;
    }

    return NULL;
}

char *updateAnComponentCatalog(char *cmp_type, char *cmp_datatype, \
        char *cmp_format, bool isMinPresent, double cmp_minvalue, bool isMaxPresent, double cmp_maxvalue,
        char *cmp_unit, char *cmp_display, char *cmp_command) {

// TODO: TODO: TODO: TODO: THIS DOES NOT SUPPORT ACTUATOR AS OF NOW

    struct curl_slist *headers = NULL;
    char *url;
    char body[BODY_SIZE_MIN];
    char *response = NULL;
    bool isCommaRequired = false;

    if(prepareUrl(&url, configurations.base_url, configurations.update_an_cmp_catalog)) {
        appendHttpHeader(&headers, HEADER_CONTENT_TYPE_NAME, HEADER_CONTENT_TYPE_JSON);
        appendHttpHeader(&headers, HEADER_AUTHORIZATION, getConfigAuthorizationToken());

        strcpy(body, "{");

        if(cmp_type != NULL) {
            strcat(body, "\"type\":\"");
            strcat(body, cmp_type);
            strcat(body, "\"");
            isCommaRequired = true;
        }

        if(cmp_datatype != NULL) {
            if(isCommaRequired) {
                strcat(body, ",");
            }
            strcat(body, "\"dataType\":\"");
            strcat(body, cmp_datatype);
            strcat(body, "\"");
            isCommaRequired = true;
        }

        if(cmp_format != NULL) {
            if(isCommaRequired) {
                strcat(body, ",");
            }
            strcat(body, "\"format\":\"");
            strcat(body, cmp_format);
            strcat(body, "\"");
            isCommaRequired = true;
        }

        char value[BODY_SIZE_MIN];
        if(isMinPresent == true) {
            sprintf(value, "%f", cmp_minvalue);
            if(isCommaRequired) {
                strcat(body, ",");
            }
            strcat(body, "\"min\":");
            strcat(body, value);
            isCommaRequired = true;
        }

        if(isMaxPresent == true) {
            sprintf(value, "%f", cmp_maxvalue);
            if(isCommaRequired) {
                strcat(body, ",");
            }
            strcat(body, "\"max\":");
            strcat(body, value);
            isCommaRequired = true;
        }

        if(cmp_unit != NULL) {
            if(isCommaRequired) {
                strcat(body, ",");
            }
            strcat(body, "\"measureunit\":\"");
            strcat(body, cmp_unit);
            strcat(body, "\"");
            isCommaRequired = true;
        }

        if(cmp_display != NULL) {
            if(isCommaRequired) {
                strcat(body, ",");
            }
            strcat(body, "\"display\":\"");
            strcat(body, cmp_display);
            strcat(body, "\"");
            isCommaRequired = true;
        }

        if(cmp_command != NULL) {
            // TODO: TODO: NEED TO BE VALIDATED FOR ACTUATOR
            if(isCommaRequired) {
                strcat(body, ",");
            }
            strcat(body, "\"command\":\"");
            strcat(body, cmp_command);
            strcat(body, "\"");
        }

        strcat(body, "}");

        #if DEBUG
            printf("Prepared BODY is %s\n", body);
        #endif

        doHttpPut(url, headers, body, &response);

        return response;
    }

    return NULL;
}
