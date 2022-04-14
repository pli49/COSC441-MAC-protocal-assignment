cplusplus {{
#include "AppMessageStatus_m.h"
}}

enum AppMessageStatus;

message AppResponse {
    int      sequenceNumber;
    int      outcome @enum(AppMessageStatus);
}