/*
Generic implementation of the s3eGameCircle extension.
This file should perform any platform-indepedentent functionality
(e.g. error checking) before calling platform-dependent implementations.
*/

/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */


#include "s3eGameCircle_internal.h"
s3eResult s3eGameCircleInit()
{
    //Add any generic initialisation code here
    return s3eGameCircleInit_platform();
}

void s3eGameCircleTerminate()
{
    //Add any generic termination code here
    s3eGameCircleTerminate_platform();
}

void s3eGameCircleInitialize(bool useAchievements, bool useWhispersync, bool useLeaderboards, bool firsRun)
{
    s3eGameCircleInitialize_platform(useAchievements, useWhispersync, useLeaderboards, firsRun);
}

s3eGameCircleStatus s3eGameCircleGetStatus()
{
    return s3eGameCircleGetStatus_platform();
}

bool s3eGameCircleIsReady()
{
    return s3eGameCircleIsReady_platform();
}

void s3eGameCircleGetPlayerAlias(const char* developerPayload)
{
    s3eGameCircleGetPlayerAlias_platform(developerPayload);
}

void s3eGameCircleShowAchievementsOverlay()
{
    s3eGameCircleShowAchievementsOverlay_platform();
}

void s3eGameCircleUpdateAchievement(const char* achievementId, float percentComplete, const char* developerPayload)
{
    s3eGameCircleUpdateAchievement_platform(achievementId, percentComplete, developerPayload);
}

void s3eGameCircleResetAchievements()
{
    s3eGameCircleResetAchievements_platform();
}

void s3eGameCircleResetAchievement(const char* achievementId, const char* developerPayload)
{
    s3eGameCircleResetAchievement_platform(achievementId, developerPayload);
}

void s3eGameCircleSetPopUpLocation(s3eGameCirclePopUpLocation location)
{
    s3eGameCircleSetPopUpLocation_platform(location);
}

void s3eGameCircleShowLeaderboardOverlay(const char* leaderboardId)
{
    s3eGameCircleShowLeaderboardOverlay_platform(leaderboardId);
}

void s3eGameCircleShowLeaderboardsOverlay()
{
    s3eGameCircleShowLeaderboardsOverlay_platform();
}

void s3eGameCircleSubmitScore(const char* leaderboardId, int64_t score, const char* developerPayload)
{
    s3eGameCircleSubmitScore_platform(leaderboardId, score, developerPayload);
}

void s3eGameCircleGetLeaderboards(const char* developerPayload)
{
    s3eGameCircleGetLeaderboards_platform(developerPayload);
}

void s3eGameCircleGetScores(const char* leaderboardId, s3eGameCircleLeaderboardFilter filter, int startRank, int count, const char* developerPayload)
{
    s3eGameCircleGetScores_platform(leaderboardId, filter, startRank, count, developerPayload);
}

void s3eGameCircleGetLocalPlayerScore(const char* leaderboardId, s3eGameCircleLeaderboardFilter filter, const char* developerPayload)
{
    s3eGameCircleGetLocalPlayerScore_platform(leaderboardId, filter, developerPayload);
}

bool s3eGameCircleHasNewMultiFileGameData()
{
    return s3eGameCircleHasNewMultiFileGameData_platform();
}

void s3eGameCircleUnpackNewMultiFileGameData()
{
    s3eGameCircleUnpackNewMultiFileGameData_platform();
}

void s3eGameCircleSynchronizeBlob(s3eGameCircleConflictStrategy conflictStrategy)
{
    s3eGameCircleSynchronizeBlob_platform(conflictStrategy);
}

void s3eGameCirclenchronizeBlobProgress(const char* description, const void* data, int dataLen, s3eGameCircleConflictStrategy conflictStrategy)
{
    s3eGameCirclenchronizeBlobProgress_platform(description, data, dataLen, conflictStrategy);
}

void s3eGameCircleSynchronizeMultiFile(s3eGameCircleConflictStrategy conflictStrategy)
{
    s3eGameCircleSynchronizeMultiFile_platform(conflictStrategy);
}

void s3eGameCircleSynchronizeMultiFileProgress(const char* description, s3eGameCircleConflictStrategy conflictStrategy)
{
    s3eGameCircleSynchronizeMultiFileProgress_platform(description, conflictStrategy);
}

void s3eGameCircleRequestRevertBlob()
{
    s3eGameCircleRequestRevertBlob_platform();
}

void s3eGameCircleRequestRevertMultiFile()
{
    s3eGameCircleRequestRevertMultiFile_platform();
}
