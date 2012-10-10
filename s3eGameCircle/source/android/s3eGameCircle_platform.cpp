/*
 * android-specific implementation of the s3eGameCircle extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "s3eGameCircle_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

static jobject g_Obj;
static jmethodID g_s3eGameCircleInitialize;
static jmethodID g_s3eGameCircleGetStatus;
static jmethodID g_s3eGameCircleIsReady;
static jmethodID g_s3eGameCircleGetPlayerAlias;
static jmethodID g_s3eGameCircleShowAchievementsOverlay;
static jmethodID g_s3eGameCircleUpdateAchievement;
static jmethodID g_s3eGameCircleResetAchievements;
static jmethodID g_s3eGameCircleResetAchievement;
static jmethodID g_s3eGameCircleSetPopUpLocation;
static jmethodID g_s3eGameCircleShowLeaderboardOverlay;
static jmethodID g_s3eGameCircleShowLeaderboardsOverlay;
static jmethodID g_s3eGameCircleSubmitScore;
static jmethodID g_s3eGameCircleGetLeaderboards;
static jmethodID g_s3eGameCircleGetScores;
static jmethodID g_s3eGameCircleGetLocalPlayerScore;
static jmethodID g_s3eGameCircleHasNewMultiFileGameData;
static jmethodID g_s3eGameCircleUnpackNewMultiFileGameData;
static jmethodID g_s3eGameCircleSynchronizeBlob;
static jmethodID g_s3eGameCirclenchronizeBlobProgress;
static jmethodID g_s3eGameCircleSynchronizeMultiFile;
static jmethodID g_s3eGameCircleSynchronizeMultiFileProgress;
static jmethodID g_s3eGameCircleRequestRevertBlob;
static jmethodID g_s3eGameCircleRequestRevertMultiFile;


void JNICALL amazonGameCircle_onBlobCallBack(JNIEnv* env, jobject obj, jint status, jint errorCode, jbyteArray data);
void JNICALL amazonGameCircle_onGetPlayerAlias(JNIEnv* env, jobject obj, jstring alias);
void JNICALL amazonGameCircle_onInitializeCallback(JNIEnv* env, jobject obj, jint status);

s3eResult s3eGameCircleInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;

    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("s3eGameCircle");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_s3eGameCircleInitialize = env->GetMethodID(cls, "s3eGameCircleInitialize", "(ZZZZ)V");
    if (!g_s3eGameCircleInitialize)
        goto fail;

    g_s3eGameCircleGetStatus = env->GetMethodID(cls, "s3eGameCircleGetStatus", "()I");
    if (!g_s3eGameCircleGetStatus)
        goto fail;

    g_s3eGameCircleIsReady = env->GetMethodID(cls, "s3eGameCircleIsReady", "()Z");
    if (!g_s3eGameCircleIsReady)
        goto fail;

    g_s3eGameCircleGetPlayerAlias = env->GetMethodID(cls, "s3eGameCircleGetPlayerAlias", "(Ljava/lang/String;)V");
    if (!g_s3eGameCircleGetPlayerAlias)
        goto fail;

    g_s3eGameCircleShowAchievementsOverlay = env->GetMethodID(cls, "s3eGameCircleShowAchievementsOverlay", "()V");
    if (!g_s3eGameCircleShowAchievementsOverlay)
        goto fail;

    g_s3eGameCircleUpdateAchievement = env->GetMethodID(cls, "s3eGameCircleUpdateAchievement", "(Ljava/lang/String;FLjava/lang/String;)V");
    if (!g_s3eGameCircleUpdateAchievement)
        goto fail;

    g_s3eGameCircleResetAchievements = env->GetMethodID(cls, "s3eGameCircleResetAchievements", "()V");
    if (!g_s3eGameCircleResetAchievements)
        goto fail;

    g_s3eGameCircleResetAchievement = env->GetMethodID(cls, "s3eGameCircleResetAchievement", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_s3eGameCircleResetAchievement)
        goto fail;

    g_s3eGameCircleSetPopUpLocation = env->GetMethodID(cls, "s3eGameCircleSetPopUpLocation", "()V");
    if (!g_s3eGameCircleSetPopUpLocation)
        goto fail;

    g_s3eGameCircleShowLeaderboardOverlay = env->GetMethodID(cls, "s3eGameCircleShowLeaderboardOverlay", "(Ljava/lang/String;)V");
    if (!g_s3eGameCircleShowLeaderboardOverlay)
        goto fail;

    g_s3eGameCircleShowLeaderboardsOverlay = env->GetMethodID(cls, "s3eGameCircleShowLeaderboardsOverlay", "()V");
    if (!g_s3eGameCircleShowLeaderboardsOverlay)
        goto fail;

    g_s3eGameCircleSubmitScore = env->GetMethodID(cls, "s3eGameCircleSubmitScore", "(Ljava/lang/String;JLjava/lang/String;)V");
    if (!g_s3eGameCircleSubmitScore)
        goto fail;

    g_s3eGameCircleGetLeaderboards = env->GetMethodID(cls, "s3eGameCircleGetLeaderboards", "(Ljava/lang/String;)V");
    if (!g_s3eGameCircleGetLeaderboards)
        goto fail;

    g_s3eGameCircleGetScores = env->GetMethodID(cls, "s3eGameCircleGetScores", "(Ljava/lang/String;IILjava/lang/String;)V");
    if (!g_s3eGameCircleGetScores)
        goto fail;

    g_s3eGameCircleGetLocalPlayerScore = env->GetMethodID(cls, "s3eGameCircleGetLocalPlayerScore", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!g_s3eGameCircleGetLocalPlayerScore)
        goto fail;

    g_s3eGameCircleHasNewMultiFileGameData = env->GetMethodID(cls, "s3eGameCircleHasNewMultiFileGameData", "()Z");
    if (!g_s3eGameCircleHasNewMultiFileGameData)
        goto fail;

    g_s3eGameCircleUnpackNewMultiFileGameData = env->GetMethodID(cls, "s3eGameCircleUnpackNewMultiFileGameData", "()V");
    if (!g_s3eGameCircleUnpackNewMultiFileGameData)
        goto fail;

    g_s3eGameCircleSynchronizeBlob = env->GetMethodID(cls, "s3eGameCircleSynchronizeBlob", "(I)V");
    if (!g_s3eGameCircleSynchronizeBlob)
        goto fail;

    g_s3eGameCirclenchronizeBlobProgress = env->GetMethodID(cls, "s3eGameCirclenchronizeBlobProgress", "(Ljava/lang/String;[BI)V");
    if (!g_s3eGameCirclenchronizeBlobProgress)
        goto fail;

    g_s3eGameCircleSynchronizeMultiFile = env->GetMethodID(cls, "s3eGameCircleSynchronizeMultiFile", "()V");
    if (!g_s3eGameCircleSynchronizeMultiFile)
        goto fail;

    g_s3eGameCircleSynchronizeMultiFileProgress = env->GetMethodID(cls, "s3eGameCircleSynchronizeMultiFileProgress", "(Ljava/lang/String;)V");
    if (!g_s3eGameCircleSynchronizeMultiFileProgress)
        goto fail;

    g_s3eGameCircleRequestRevertBlob = env->GetMethodID(cls, "s3eGameCircleRequestRevertBlob", "()V");
    if (!g_s3eGameCircleRequestRevertBlob)
        goto fail;

    g_s3eGameCircleRequestRevertMultiFile = env->GetMethodID(cls, "s3eGameCircleRequestRevertMultiFile", "()V");
    if (!g_s3eGameCircleRequestRevertMultiFile)
        goto fail;

    {
        //jclass receiver=s3eEdkAndroidFindClass("com/ideaworks3d/marmalade/s3eAndroidMarketBilling/s3eAndroidMarketBillingReceiver");
        static const JNINativeMethod methods[]=
        {
            {"native_onBlobCallBack","(II[B)V",(void*)&amazonGameCircle_onBlobCallBack},
            {"native_onGetPlayerAlias","(Ljava/lang/String;)V",(void*)&amazonGameCircle_onGetPlayerAlias},
            {"native_onInitializeCallback","(I)V",(void*)&amazonGameCircle_onInitializeCallback},
//            {"native_onPurchaseUpdateResponse","(ILjava/lang/String;Ljava/lang/String;[Ljava/lang/String;[Lcom/amazon/inapp/purchasing/Receipt;Ljava/lang/String;Z)V",(void*)&amazonBilling_onPurchaseUpdateResponse},
//            {"native_onItemDataResponse","(ILjava/lang/String;[Ljava/lang/String;[Lcom/amazon/inapp/purchasing/Item;)V",(void*)&amazonBilling_onItemDataResponse},
            
//            {"native_onInAppNotifyCallback","(Ljava/lang/String;)V",(void*)&InAppBilling_onInAppNotifyCallback},
//            {"native_onPurchaseStateChangedCallback","([Lcom/ideaworks3d/marmalade/s3eAndroidMarketBilling/s3eAndroidMarketBillingReceiver$Order;I)V",(void*)&InAppBilling_onPurchaseStateChangedCallback}
        };
        // Register the native hooks
        if(env->RegisterNatives(cls, methods, sizeof(methods)/sizeof(methods[0])))
            goto fail;
    }


    IwTrace(GAMECIRCLE, ("GAMECIRCLE init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(s3eGameCircle, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}

void s3eGameCircleTerminate_platform()
{
    // Add any platform-specific termination code here
}

void s3eGameCircleInitialize_platform(bool useAchievements, bool useWhispersync, bool useLeaderboards, bool firsRun)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleInitialize, useAchievements, useWhispersync, useLeaderboards, firsRun);
}

s3eGameCircleStatus s3eGameCircleGetStatus_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jint res = env->CallIntMethod(g_Obj, g_s3eGameCircleGetStatus);
    return (s3eGameCircleStatus)res;
}

bool s3eGameCircleIsReady_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (bool)env->CallBooleanMethod(g_Obj, g_s3eGameCircleIsReady);
}

void s3eGameCircleGetPlayerAlias_platform(const char* developerPayload)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring developerPayload_jstr = env->NewStringUTF(developerPayload);
    env->CallVoidMethod(g_Obj, g_s3eGameCircleGetPlayerAlias, developerPayload_jstr);
}

void s3eGameCircleShowAchievementsOverlay_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleShowAchievementsOverlay);
}

void s3eGameCircleUpdateAchievement_platform(const char* achievementId, float percentComplete, const char* developerPayload)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring achievementId_jstr = env->NewStringUTF(achievementId);
    jstring developerPayload_jstr = env->NewStringUTF(developerPayload);
    env->CallVoidMethod(g_Obj, g_s3eGameCircleUpdateAchievement, achievementId_jstr, percentComplete, developerPayload_jstr);
}

void s3eGameCircleResetAchievements_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleResetAchievements);
}

void s3eGameCircleResetAchievement_platform(const char* achievementId, const char* developerPayload)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring achievementId_jstr = env->NewStringUTF(achievementId);
    jstring developerPayload_jstr = env->NewStringUTF(developerPayload);
    env->CallVoidMethod(g_Obj, g_s3eGameCircleResetAchievement, achievementId_jstr, developerPayload_jstr);
}

void s3eGameCircleSetPopUpLocation_platform(s3eGameCirclePopUpLocation location)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleSetPopUpLocation);
}

void s3eGameCircleShowLeaderboardOverlay_platform(const char* leaderboardId)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring leaderboardId_jstr = env->NewStringUTF(leaderboardId);
    env->CallVoidMethod(g_Obj, g_s3eGameCircleShowLeaderboardOverlay, leaderboardId_jstr);
}

void s3eGameCircleShowLeaderboardsOverlay_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleShowLeaderboardsOverlay);
}

void s3eGameCircleSubmitScore_platform(const char* leaderboardId, int64_t score, const char* developerPayload)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring leaderboardId_jstr = env->NewStringUTF(leaderboardId);
    jstring developerPayload_jstr = env->NewStringUTF(developerPayload);
    env->CallVoidMethod(g_Obj, g_s3eGameCircleSubmitScore, leaderboardId_jstr, score, developerPayload_jstr);
}

void s3eGameCircleGetLeaderboards_platform(const char* developerPayload)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring developerPayload_jstr = env->NewStringUTF(developerPayload);
    env->CallVoidMethod(g_Obj, g_s3eGameCircleGetLeaderboards, developerPayload_jstr);
}

void s3eGameCircleGetScores_platform(const char* leaderboardId, s3eGameCircleLeaderboardFilter filter, int startRank, int count, const char* developerPayload)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring leaderboardId_jstr = env->NewStringUTF(leaderboardId);
    jstring developerPayload_jstr = env->NewStringUTF(developerPayload);
    env->CallVoidMethod(g_Obj, g_s3eGameCircleGetScores, leaderboardId_jstr, startRank, count, developerPayload_jstr);
}

void s3eGameCircleGetLocalPlayerScore_platform(const char* leaderboardId, s3eGameCircleLeaderboardFilter filter, const char* developerPayload)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring leaderboardId_jstr = env->NewStringUTF(leaderboardId);
    jstring developerPayload_jstr = env->NewStringUTF(developerPayload);
    env->CallVoidMethod(g_Obj, g_s3eGameCircleGetLocalPlayerScore, leaderboardId_jstr, developerPayload_jstr);
}

bool s3eGameCircleHasNewMultiFileGameData_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    return (bool)env->CallBooleanMethod(g_Obj, g_s3eGameCircleHasNewMultiFileGameData);
}

void s3eGameCircleUnpackNewMultiFileGameData_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleUnpackNewMultiFileGameData);
}

void s3eGameCircleSynchronizeBlob_platform(s3eGameCircleConflictStrategy conflictStrategy)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleSynchronizeBlob, (int)conflictStrategy);
}

void s3eGameCirclenchronizeBlobProgress_platform(const char* description, const void* data, int dataLen, s3eGameCircleConflictStrategy conflictStrategy)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring description_jstr = env->NewStringUTF(description);
    jbyteArray dataArray = env->NewByteArray(dataLen);
    env->SetByteArrayRegion(dataArray, 0, dataLen, (jbyte*)data);

    env->CallVoidMethod(g_Obj, g_s3eGameCirclenchronizeBlobProgress, description_jstr, dataArray, (int)conflictStrategy);
}

void s3eGameCircleSynchronizeMultiFile_platform(s3eGameCircleConflictStrategy conflictStrategy)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleSynchronizeMultiFile);
}

void s3eGameCircleSynchronizeMultiFileProgress_platform(const char* description, s3eGameCircleConflictStrategy conflictStrategy)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring description_jstr = env->NewStringUTF(description);
    env->CallVoidMethod(g_Obj, g_s3eGameCircleSynchronizeMultiFileProgress, description_jstr);
}

void s3eGameCircleRequestRevertBlob_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleRequestRevertBlob);
}

void s3eGameCircleRequestRevertMultiFile_platform()
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    env->CallVoidMethod(g_Obj, g_s3eGameCircleRequestRevertMultiFile);
}


static char * javaStringToNative(jstring javastr) {
    if (!javastr) return NULL;
    JNIEnv* env = s3eEdkJNIGetEnv();
    const char* utf = env->GetStringUTFChars(javastr, NULL);
    jsize utfLen = env->GetStringLength(javastr);

    char *native = new char[utfLen+1];
    memcpy(native, utf, utfLen+1); //faster strcpy, since length is known

    env->ReleaseStringUTFChars(javastr, utf);
    return native;
}



static void amazonGameCircle_onBlobCallBackClean(uint32, int32, void*systemData, void*, int32, void*) {
    s3eGameCircleBlobResponse* response = (s3eGameCircleBlobResponse*)systemData;
    if (response)  {
        if (response->m_Data) {
            char* d = (char*)response->m_Data;
            delete[] d;
        }
    }
}


void JNICALL amazonGameCircle_onBlobCallBack(JNIEnv* env, jobject obj, jint status, jint errorCode, jbyteArray data) {
    
    s3eGameCircleBlobResponse response;
    response.m_Status = (s3eGameCircleBlobCallbackStatus)status;
    response.m_ErrorCode = (s3eGameCircleErrorCode)errorCode;
    response.m_Data = 0;
    response.m_DataLen = 0;

    if (data) {
        response.m_DataLen = env->GetArrayLength(data);
        char* nativeData = new char[response.m_DataLen];
        env->GetByteArrayRegion(data, 0, response.m_DataLen, (jbyte*)nativeData);
        response.m_Data = nativeData;
    }

    s3eEdkCallbacksEnqueue(S3E_EXT_GAMECIRCLE_HASH, S3E_GAMECIRCLE_SYNCHRONIZE_BLOB_CALLBACK, &response,
                           sizeof(response), NULL, false,
                           &amazonGameCircle_onBlobCallBackClean, 0);
}

void JNICALL amazonGameCircle_onGetPlayerAlias(JNIEnv* env, jobject obj, jstring alias) {
    char* aliasNative = javaStringToNative( alias );
    
    s3eEdkCallbacksEnqueue(S3E_EXT_GAMECIRCLE_HASH, S3E_GAMECIRCLE_PLAYER_ALIAS_CALLBACK, aliasNative,
                           strlen(aliasNative)+1, NULL, false,
                           0, 0);

}

void JNICALL amazonGameCircle_onInitializeCallback(JNIEnv* env, jobject obj, jint status) {
    
    s3eEdkCallbacksEnqueue(S3E_EXT_GAMECIRCLE_HASH, S3E_GAMECIRCLE_INIT_CALLBACK, (void*)status, 0, NULL, false, 0, 0);
}
