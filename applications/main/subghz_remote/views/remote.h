#pragma once

#include <gui/view.h>
#include "../helpers/subrem_custom_event.h"

typedef enum {
    SubRemViewRemoteStateIdle,
    SubRemViewRemoteStateLoading,
    SubRemViewRemoteStateSending,
} SubRemViewRemoteState;

typedef struct SubRemViewRemote SubRemViewRemote;

typedef void (*SubRemViewRemoteCallback)(SubRemCustomEvent event, void* context);

void subrem_view_remote_set_callback(
    SubRemViewRemote* subrem_view_remote,
    SubRemViewRemoteCallback callback,
    void* context);

SubRemViewRemote* subrem_view_remote_alloc();

void subrem_view_remote_free(SubRemViewRemote* subrem_view_remote);

View* subrem_view_remote_get_view(SubRemViewRemote* subrem_view_remote);

void subrem_view_remote_add_data_to_show(
    SubRemViewRemote* subrem_view_remote,
    const char* up_label,
    const char* down_label,
    const char* left_label,
    const char* right_label,
    const char* ok_label);

void subrem_view_remote_set_presed_btn(SubRemViewRemote* subrem_view_remote, uint8_t presed_btn);
void subrem_view_remote_set_state(
    SubRemViewRemote* subrem_view_remote,
    SubRemViewRemoteState state);