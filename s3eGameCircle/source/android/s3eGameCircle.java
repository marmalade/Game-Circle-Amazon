/*
java implementation of the s3eGameCircle extension.

Add android-specific functionality here.

These functions are called via JNI from native code.
*/
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
import com.ideaworks3d.marmalade.LoaderAPI;
import com.ideaworks3d.marmalade.LoaderActivity;
import com.amazon.ags.api.*;
import com.amazon.ags.constants.*;
import com.amazon.ags.constants.whispersync.*;
import com.amazon.ags.api.whispersync.*;
import com.amazon.ags.api.profiles.*;
import com.amazon.ags.api.leaderboards.SubmitScoreResponse;

import android.util.Log;
import java.util.EnumSet;

class s3eGameCircle {
    private static final String TAG = "Amazon-GameCircle";

    private SynchronizeBlobCallback blobCallback = null;
    
    public void s3eGameCircleInitialize(boolean useAchievements, final boolean useWhispersync, boolean useLeaderboards, final boolean firsRun) {
        Log.v(TAG, "s3eGameCircleInitialize first: " + firsRun);
        //Make a list of the Amazon Game Services features your game uses
        //only show the features you are using.
        EnumSet<AmazonGamesFeature> myGameFeatures = EnumSet.noneOf(AmazonGamesFeature.class);
        if (useAchievements) myGameFeatures.add(AmazonGamesFeature.Achievements);
        if (useWhispersync) myGameFeatures.add(AmazonGamesFeature.Whispersync);
        if (useLeaderboards) myGameFeatures.add(AmazonGamesFeature.Leaderboards);
        
        AmazonGamesClient.initialize(LoaderActivity.m_Activity.getApplication(),
            new AmazonGamesCallback() {
                    @Override
                    public void onServiceReady() {
                        Log.v(TAG, "s3eGameCircleInitialize service is ready! " + AmazonGamesStatus.SERVICE_CONNECTED.ordinal());
                        int status = AmazonGamesClient.getInstance().getStatus().ordinal();
                        native_onInitializeCallback(status);
                        //native_onInitializeCallback(AmazonGamesStatus.SERVICE_CONNECTED.ordinal());
                        if (useWhispersync) {
                            SynchronizeBlobRequest syncBlobRequest = new SynchronizeBlobRequest(blobCallback);
                            if (firsRun) {
                                syncBlobRequest.setConflictStrategy(ConflictStrategy.AUTO_RESOLVE_TO_CLOUD);
                            }
                            AmazonGamesClient.getInstance().getWhisperSyncClient().synchronize(syncBlobRequest);
                        }

                    }
                    @Override
                    public void onServiceNotReady(AmazonGamesStatus reason) {
                        Log.v(TAG, "s3eGameCircleInitialize service failed!: " + reason);
                        switch (reason) {
                        /**
                         * The service could not bind either because it does not
                         * exist, or permissions have not been granted.
                         * This will also occur when your game is executed on a non-fire device
                         * that does not have Amazon Game Services installedAmazon.com Confidential Do  Not Distribute
                         */
                        case CANNOT_BIND:
                        /**
                         * The service could not authorize the client. This
                         * should only occur if the network is not available the
                         * first time the game attempts to connect.
                         */
                        case CANNOT_AUTHORIZE:
                        /**
                         * The game in not authorized to use the service. Check
                         * your package name and signature registered in the
                         * Developer's Portal
                         */
                        case NOT_AUTHORIZED:
                        /**
                         * The device is not registered with an account
                         */
                        case NOT_AUTHENTICATED:
                        /**
                         * Disable Amazon Game Services features in Game UI
                         */
                        break;
                        }
                        //int status = AmazonGamesClient.getInstance().getStatus().ordinal();
                        native_onInitializeCallback(reason.ordinal());
                    }
                }, myGameFeatures); 
        if (useWhispersync) {
            this.blobCallback = new SynchronizeBlobCallback() {
                        @Override
                        public void onAlreadySynchronized() {
                            // nothing to do, data is already in sync
                            native_onBlobCallBack(0, 0, null);
                        }
                        @Override
                        public void onConflictDeferral() {
                            // a conflict was encountered, but the gamer chose to ignore it
                            native_onBlobCallBack(1, 0, null);
                        }
                        @Override
                        public void onGameUploadSuccess() {
                            // current data has been synchronized
                            native_onBlobCallBack(2, 0, null);
                        }
                        @Override
                        public void onSynchronizeFailure(ErrorCode errorCode) {
                            // sync failed
                            native_onBlobCallBack(3, errorCode.ordinal(), null);
                        }
                        @Override
                        public boolean onNewGameData(byte[] data) {
                            //unpack your data               
                            native_onBlobCallBack(4, 0, data);
                            return true;
                        }
            };
        }
    }
    
    private static native void native_onBlobCallBack(int status, int errorCode, byte[] data);
    private static native void native_onGetPlayerAlias(String alias);
    private static native void native_onInitializeCallback(int status);

    public int s3eGameCircleGetStatus() {
        AmazonGames client = AmazonGamesClient.getInstance();
        if (client != null) {
            return client.getStatus().ordinal();
        }
        return AmazonGamesStatus.INITIALIZING.ordinal();
    }

    public boolean s3eGameCircleIsReady() {
        AmazonGames client = AmazonGamesClient.getInstance();
        if (client != null) {
            return client.isReady();
        }
        return false;
    }

    public void s3eGameCircleGetPlayerAlias(String developerPayload) {
        Log.v(TAG, "s3eGameCircleGetPlayerAlias");
        if (AmazonGamesClient.getInstance() == null) return;
        AmazonGamesClient.getInstance().getProfilesClient().getLocalPlayerProfile().setCallback(new
            AGResponseCallback<RequestPlayerProfileResponse>() {
                @Override
                public void onComplete(RequestPlayerProfileResponse result) {
                    if (result.isError()) {
                    // Add optional error handling here.  Not required since re-tries and on-device request caching are automatic
                        Log.v(TAG, "s3eGameCircleGetPlayerAlias ERROR: " + result.getError());
                    } else {
                        Log.v(TAG, "s3eGameCircleGetPlayerAlias OK");
                        // Continue game flow
                        String alias = result.getPlayer().getAlias();
                        Log.v(TAG, "alias: " + alias);
                        native_onGetPlayerAlias(alias);
                    }
                }
            });

        
    }

    public void s3eGameCircleShowAchievementsOverlay() {
        Log.v(TAG, "s3eGameCircleShowAchievementsOverlay");
        AmazonGamesClient.getInstance().getAchievementsClient().showAchievementsOverlay();
    }

    public void s3eGameCircleUpdateAchievement(String achievementId, float percentComplete, String developerPayload) {
        Log.v(TAG, "s3eGameCircleUpdateAchievement " + achievementId + ' ' + percentComplete + ' ' + developerPayload);
        if (AmazonGamesClient.getInstance() == null) return;
        AmazonGamesClient.getInstance().getAchievementsClient().updateProgress(achievementId, percentComplete, developerPayload);
    }

    public void s3eGameCircleResetAchievements() {
        Log.v(TAG, "s3eGameCircleResetAchievements");
        if (AmazonGamesClient.getInstance() == null) return;
        AmazonGamesClient.getInstance().getAchievementsClient().resetAchievements().setCallback(new
            AGResponseCallback<RequestResponse>() {
                @Override
                public void onComplete(RequestResponse result) {
                    if (result.isError()) {
                    // Add optional error handling here.  Not required since re-tries and on-device request caching are automatic
                        Log.v(TAG, "s3eGameCircleResetAchievements ERROR: " + result.getError());
                    } else {
                        Log.v(TAG, "s3eGameCircleResetAchievements OK");
                        // Continue game flow
                    }
                }
            });
    }

    public void s3eGameCircleResetAchievement(String achievementId, String developerPayload) {
        Log.v(TAG, "s3eGameCircleResetAchievement " + achievementId);
        if (AmazonGamesClient.getInstance() == null) return;
        AmazonGamesClient.getInstance().getAchievementsClient().resetAchievement(achievementId, developerPayload).setCallback(new
            AGResponseCallback<RequestResponse>() {
                @Override
                public void onComplete(RequestResponse result) {
                    if (result.isError()) {
                    // Add optional error handling here.  Not required since re-tries and on-device request caching are automatic
                        Log.v(TAG, "s3eGameCircleResetAchievement ERROR: " + result.getError());
                    } else {
                        Log.v(TAG, "s3eGameCircleResetAchievement OK");
                        // Continue game flow
                    }
                }
            });
    }

    public void s3eGameCircleSetPopUpLocation() {

    }

    public void s3eGameCircleShowLeaderboardOverlay(String leaderboardId) {
        Log.v(TAG, "s3eGameCircleShowLeaderboardOverlay " + leaderboardId);
        if (AmazonGamesClient.getInstance() == null) return;
        AmazonGamesClient.getInstance().getLeaderboardsClient().showLeaderboardOverlay(leaderboardId);
    }

    public void s3eGameCircleShowLeaderboardsOverlay() {
        Log.v(TAG, "s3eGameCircleShowLeaderboardsOverlay");
        if (AmazonGamesClient.getInstance() == null) return;
        AmazonGamesClient.getInstance().getLeaderboardsClient().showLeaderboardsOverlay();
    }
    
    public void s3eGameCircleSubmitScore(String leaderboardId, long score, String developerPayload) {
        Log.v(TAG, "s3eGameCircleSubmitScore " + leaderboardId + ' ' + score);
        if (AmazonGamesClient.getInstance() == null) return;
        AmazonGamesClient.getInstance().getLeaderboardsClient().submitScore(leaderboardId, score, developerPayload).setCallback(new
            AGResponseCallback<SubmitScoreResponse>() {
                @Override
                public void onComplete(SubmitScoreResponse result) {
                    if (result.isError()) {
                        // Add optional error handling here.  Not required since re-tries and on-device request caching are automatic
                    // Add optional error handling here.  Not required since re-tries and on-device request caching are automatic
                        Log.v(TAG, "s3eGameCircleSubmitScore ERROR: " + result.getError());
                    } else {
                        Log.v(TAG, "s3eGameCircleSubmitScore OK");
                      // Continue game flow
                    }
                }
        });
    }

    public void s3eGameCircleGetLeaderboards(String developerPayload) {

    }
    
    public void s3eGameCircleGetScores(String leaderboardId, int startRank, int count, String developerPayload) {
        
    }
    
    public void s3eGameCircleGetLocalPlayerScore(String leaderboardId, String developerPayload) {
        
    }
    
    public boolean s3eGameCircleHasNewMultiFileGameData() {
        Log.v(TAG, "s3eGameCircleHasNewMultiFileGameData");
        if (AmazonGamesClient.getInstance() == null) return false;
        return AmazonGamesClient.getInstance().getWhisperSyncClient().hasNewMultiFileGameData();
    }
    
    public void s3eGameCircleUnpackNewMultiFileGameData() {
        Log.v(TAG, "s3eGameCircleHasNewMultiFileGameData");
        if (AmazonGamesClient.getInstance() == null) return;
        try {
            AmazonGamesClient.getInstance().getWhisperSyncClient().unpackNewMultiFileGameData();
        } catch (java.io.IOException e) {
            e.printStackTrace();
        }

    }
    
    public void s3eGameCircleSynchronizeBlob(int strategy) {
        Log.v(TAG, "s3eGameCircleSynchronizeBlob " + strategy);
        if (AmazonGamesClient.getInstance() == null || blobCallback == null) return;
        ConflictStrategy strat = ConflictStrategy.values()[ strategy ];
        SynchronizeBlobRequest br = new SynchronizeBlobRequest( this.blobCallback );
        br.setConflictStrategy(strat);
        AmazonGamesClient.getInstance().getWhisperSyncClient().synchronize(br);
    }
    
    public void s3eGameCirclenchronizeBlobProgress(String description, byte[] data, int strategy) {
        Log.v(TAG, "s3eGameCirclenchronizeBlobProgress " + description + ' ' + strategy);
        if (AmazonGamesClient.getInstance() == null || blobCallback == null) return;
        ConflictStrategy strat = ConflictStrategy.values()[ strategy ];
        SynchronizeBlobProgressRequest br = new SynchronizeBlobProgressRequest( this.blobCallback );
        br.setConflictStrategy(strat);
        br.setData(data);
        br.setDescription(description);
        AmazonGamesClient.getInstance().getWhisperSyncClient().synchronizeProgress(br);
    }
    
    public void s3eGameCircleSynchronizeMultiFile() {
        
    }
    
    public void s3eGameCircleSynchronizeMultiFileProgress(String description) {
        
    }
    
    public void s3eGameCircleRequestRevertBlob() {
        Log.v(TAG, "s3eGameCircleRequestRevertBlob");
        AmazonGamesClient.getInstance().getWhisperSyncClient().requestRevert(new RevertBlobCallback() {
            @Override
            public void onPlayerCancelled() {
                Log.v(TAG, "s3eGameCircleRequestRevertBlob onPlayerCancelled");
                     //Nothing to do here.
            }
            @Override
            public void onRevertFailure(ErrorCode error) {
                Log.v(TAG, "s3eGameCircleRequestRevertBlob onRevertFailure");
                    //Nothing to do here.  Optionally, tell the customer
            }
            @Override
            public boolean onRevertedGameData(byte[] data) {
                Log.v(TAG, "s3eGameCircleRequestRevertBlob onRevertedGameData");
                // extract data
                return true;
            }
        });

    }
    
    public void s3eGameCircleRequestRevertMultiFile() {
        
    }

}
