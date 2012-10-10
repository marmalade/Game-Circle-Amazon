/*
 *
 * Copyright (C) 2012 Funlinker.
 * All Rights Reserved.
 *
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

/**
 * @page Examples3eAmazonGameCircle S3E GameCircle Example
 *
 *
 *
 * @include s3eGameCircle.cpp
 */

#include "ExamplesMain.h"
#include "s3eGameCircle.h"
#include "s3eOSReadString.h"
#include <time.h>
#include <stdlib.h>

const char* test_achieve_id = "com_aerohills_amazo_tests_achievement_id0001";
const char* leaderboardId = "com_aerohills_test_3musketeers_leaderboard_id0001";

Button* g_ShowAchievement;
Button* g_UpdateAchieve;
Button* g_ResetAchievements;
Button* g_ShowLeaderBoards;
Button* g_PutProgress;
Button* g_Revert;
Button* g_SubmitScore;

static bool clientIsReady = false;


// Append a message based on the 
void AppendStatusMessage(s3eGameCircleStatus r) {
    switch(r) {
#define RETURN(c) case c: AppendMessage(#c); break;
        RETURN(S3E_GAMECIRCLE_CANNOT_AUTHORIZE);
        RETURN(S3E_GAMECIRCLE_CANNOT_BIND)
        RETURN(S3E_GAMECIRCLE_INITIALIZING)
        RETURN(S3E_GAMECIRCLE_INVALID_SESSION)
        RETURN(S3E_GAMECIRCLE_NOT_AUTHENTICATED)
        RETURN(S3E_GAMECIRCLE_NOT_AUTHORIZED)
        RETURN(S3E_GAMECIRCLE_SERVICE_CONNECTED)
        RETURN(S3E_GAMECIRCLE_SERVICE_DISCONNECTED)
        RETURN(S3E_GAMECIRCLE_SERVICE_NOT_OPTED_IN)
        RETURN(S3E_GAMECIRCLE_STILL_INITIALIZING)
        RETURN(S3E_GAMECIRCLE_STILL_INIT_NOT_CALLED)
#undef RETURN
    }
}


void AppendBlobStatusMessage(s3eGameCircleBlobCallbackStatus r) {
    switch(r) {
#define RETURN(c) case c: AppendMessage(#c); break;
        RETURN(S3E_GAMECIRCLE_BLOB_ALREADYSYNCHRONIZED);
        RETURN(S3E_GAMECIRCLE_BLOB_CONFLICTDEFERRAL)
        RETURN(S3E_GAMECIRCLE_BLOB_GAMEUPLOADSUCCESS)
        RETURN(S3E_GAMECIRCLE_BLOB_SYNCHRONIZEFAILURE)
        RETURN(S3E_GAMECIRCLE_BLOB_NEWGAMEDATA)
#undef RETURN
    }
}

void AppendErrorCodeMessage(s3eGameCircleErrorCode r) {
    switch(r) {
#define RETURN(c) case c: AppendMessage(#c); break;
        RETURN(S3E_GAMECIRCLE_NONE);
        RETURN(S3E_GAMECIRCLE_UNRECOVERABLE);
        RETURN(S3E_GAMECIRCLE_SERVICE_NOT_READY);
        RETURN(S3E_GAMECIRCLE_IO_ERROR);
        RETURN(S3E_GAMECIRCLE_CONNECTION_ERROR);
        RETURN(S3E_GAMECIRCLE_AUTHENTICATION_ERROR);
        RETURN(S3E_GAMECIRCLE_DATA_VALIDATION_ERROR);
#undef RETURN
    }
}

// Callback function
int32 onGetPlayerAlias(void* system, void* user) {
    printf("onGetPlayerAlias! \n");
    //    AppendMessage("onUserIdResponse:");
    AppendMessage("Alias: %s", system);
    return 0;
}

int32 onInitialize(void* system, void* user) {
    printf("onInitialize! %d\n", (int)system);
    //    AppendMessage("onUserIdResponse:");
    AppendStatusMessage((s3eGameCircleStatus)(int)system);
    AppendMessage("Initialized %d", system);
    return 0;
}

int32 onSyncronize(void* system, void* user) {
    printf("onSyncronize!\n");
    s3eGameCircleBlobResponse* response = (s3eGameCircleBlobResponse*)system;
    if (response) {
        AppendBlobStatusMessage(response->m_Status);
        if (response->m_Status == S3E_GAMECIRCLE_BLOB_SYNCHRONIZEFAILURE) {
            AppendErrorCodeMessage(response->m_ErrorCode);
        } else if (response->m_Status == S3E_GAMECIRCLE_BLOB_NEWGAMEDATA) {
            if (response->m_Data) {
                AppendMessage("sync data: %s", response->m_Data);
            }
        }
    }
    return 0;
}

void ExampleInit() {
    InitMessages(25, 256);

    // create UI buttons
    g_ShowAchievement  = NewButton("Show achievements");
    g_UpdateAchieve = NewButton("Update achieve");
    g_ResetAchievements = NewButton("Reset All achievements");
    g_ShowLeaderBoards = NewButton("Show leaderboards");
    g_PutProgress = NewButton("Put progress");
    g_Revert = NewButton("Revert");
    g_SubmitScore = NewButton("SubmitScore");

    g_ShowAchievement->m_Enabled = false;
    g_UpdateAchieve->m_Enabled = false;
    g_ResetAchievements->m_Enabled = false;
    g_ShowLeaderBoards->m_Enabled = false;
    g_PutProgress->m_Enabled = false;
    g_Revert->m_Enabled = false;
    g_SubmitScore->m_Enabled = false;

    // Check whether or not the extension is available and is ready to use
    printf("before call s3eGameCircleAvailable\n");
    if (!s3eGameCircleAvailable())
    {
        AppendMessage("Game Circle extension unavailable");
        return;
    }


    s3eGameCircleRegister(S3E_GAMECIRCLE_PLAYER_ALIAS_CALLBACK, &onGetPlayerAlias, NULL);
    s3eGameCircleRegister(S3E_GAMECIRCLE_INIT_CALLBACK, &onInitialize, NULL);
    s3eGameCircleRegister(S3E_GAMECIRCLE_SYNCHRONIZE_BLOB_CALLBACK, &onSyncronize, NULL);
    
    bool firsTime = !s3eFileCheckExists("data-ram:washere");
    if (firsTime) {
        s3eFile* file = s3eFileOpen("washere", "wb");
        s3eFileWrite("1", 1, 1, file);
    }

    printf("before call s3eGameCircleInitialize\n");
    s3eGameCircleInitialize(true, true, true, firsTime);


    // Allow user to interact
    g_ShowAchievement->m_Enabled = true;
    g_UpdateAchieve->m_Enabled = true;
    g_ResetAchievements->m_Enabled = true;
    g_ShowLeaderBoards->m_Enabled = true;
    g_PutProgress->m_Enabled = true;
    g_Revert->m_Enabled = true;
    g_SubmitScore->m_Enabled = true;
}

void ExampleTerm()
{
    if (s3eGameCircleAvailable()) {
    // Cleanup
    // Buttons are automatically deallocated by example framework
    // Unregister from callback
//    s3eAmazonBillingUnRegister(S3E_AMAZONBILLING_PURCHASE_RESPONSE, OrderInformationChanged);
        //s3eAmazonBillingTerminate();
    }
}

bool ExampleUpdate() {
    if (s3eGameCircleAvailable()) {
        if (!clientIsReady && s3eGameCircleIsReady()) {
            clientIsReady = true;
            AppendMessage("Client now is ready");
            s3eGameCircleGetPlayerAlias();
        }
    }
    Button* selectedButton = GetSelectedButton();

    if (selectedButton == g_ShowAchievement)
    {
        
        s3eGameCircleShowAchievementsOverlay();
    }
    else if (selectedButton == g_UpdateAchieve)
    {
        const char* achievePercentsText = s3eOSReadStringUTF8WithDefault("Enter an percents", "10");
        if (achievePercentsText == NULL)
            return true;
        float percents = (float)atof(achievePercentsText);
        
        // Get item id from user
        const char* achieveId = s3eOSReadStringUTF8WithDefault("Enter an achieve ID to update", test_achieve_id);
        if (achieveId == NULL)
            return true;
        printf("achieveId: %s, percents: %f\n", achieveId, percents);
        s3eGameCircleUpdateAchievement(achieveId, percents);
        printf("achieveId: %s, percents: %f\n", achieveId, percents);
        s3eGameCircleUpdateAchievement(achieveId, percents);
        printf("yop\n");
    }
    else if (selectedButton == g_ResetAchievements) {
        s3eGameCircleResetAchievements();
    }
    else if (selectedButton == g_ShowLeaderBoards) {
        s3eGameCircleShowLeaderboardsOverlay();
    }
    else if (selectedButton == g_PutProgress) {
        // Get item id from user
        const char* progress = s3eOSReadStringUTF8WithDefault("Enter a progress data to put", "SAMPLE PROGRESS DATA");
        if (progress == NULL)
            return true;
        s3eGameCirclenchronizeBlobProgress("save01", progress, strlen(progress) + 1);
    }
    else if (selectedButton == g_Revert) {
        s3eGameCircleRequestRevertBlob();
    } else if (selectedButton == g_SubmitScore) {
        const char* scoresTxt = s3eOSReadStringUTF8WithDefault("Enter a scores to submit", "10");
        if (scoresTxt == NULL)
            return true;
        int scores = atoi(scoresTxt);
        s3eGameCircleSubmitScore(leaderboardId, scores);
    }

    return true;
}

void ExampleRender() {
    ButtonsRender();
    int y = GetYBelowButtons();
    y +=  20;
    PrintMessages(10, y);
}
