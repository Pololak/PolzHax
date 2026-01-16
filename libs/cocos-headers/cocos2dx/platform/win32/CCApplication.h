#ifndef __CC_APPLICATION_WIN32_H__
#define __CC_APPLICATION_WIN32_H__

#include "ccMacros.h"
#include "CCStdC.h"
#include "platform/CCCommon.h"
#include "platform/CCApplicationProtocol.h"
#include <string>

NS_CC_BEGIN

class CCRect;

class CC_DLL CCApplication : public CCApplicationProtocol
{
public:
CCApplication();
    virtual ~CCApplication();

    /**
    @brief    Run the message loop.
    */
    // Robtop Removal
    // virtual int run();

    // @note RobTop Addition
    virtual void gameDidSave();

    /**
    @brief    Get current applicaiton instance.
    @return Current application instance pointer.
    */
    static CCApplication* sharedApplication();

    /* override functions */
    virtual void setAnimationInterval(double interval);
    virtual ccLanguageType getCurrentLanguage();

    /**
     @brief Get target platform
     */
    virtual TargetPlatform getTargetPlatform();

    virtual void openURL(const char* url);
    virtual int run();
    virtual void setupGLView();
    virtual void platformShutdown();
    void toggleVerticalSync(bool);
    // @note RobTop Addition
    void setupVerticalSync();
    // @note RobTop Addition
    void updateVerticalSync();
    // @note RobTop Addition
    void updateControllerKeys();

    // @note RobTop Addition
    int getTimeElapsed();
    // @note RobTop Addition
    void resetForceTimer();

    // @note RobTop Addition
    void leftMouseDown();
    // @note RobTop Addition
    void leftMouseUp();

    // @note RobTop Addition
    void logTimeElapsed(std::string);

    // @note RobTop Addition
    void moveMouse(int, int);

    // @note RobTop Addition
    void shutdownApplication();
    // @note RobTop Addition
    void toggleMouseControl(bool);
    // @note RobTop Addition
    void updateController();
    // @note RobTop Addition
    void updateMouseControl();


    /**
     *  Sets the Resource root path.
     *  @deprecated Please use CCFileUtils::sharedFileUtils()->setSearchPaths() instead.
     */
    CC_DEPRECATED_ATTRIBUTE void setResourceRootPath(const std::string& rootResDir);

    /**
     *  Gets the Resource root path.
     *  @deprecated Please use CCFileUtils::sharedFileUtils()->getSearchPaths() instead.
     */
    CC_DEPRECATED_ATTRIBUTE const std::string& getResourceRootPath(void);

    void setStartupScriptFilename(const std::string& startupScriptFile);

    const std::string& getStartupScriptFilename(void)
    {
        return m_startupScriptFilename;
    }

public:
    HINSTANCE           m_hInstance;
    HACCEL              m_hAccelTable;
    LARGE_INTEGER       m_nAnimationInterval;
    LARGE_INTEGER       m_nVsyncInterval;
    std::string          m_resourceRootPath;
    std::string          m_startupScriptFilename;
    void* m_pControllerHandler;
    bool m_bUpdateController;
    CC_SYNTHESIZE_NV(bool, m_bShutdownCalled, ShutdownCalled);
    INPUT m_iInput;
    CCPoint m_obLeftThumb;
    CCPoint m_obRightThumb;
    bool m_bMouseControl;
    float m_fAnimationInterval;
    float m_fVsyncInterval;
    CC_SYNTHESIZE_READONLY_NV(bool, m_bVerticalSyncEnabled, VerticalSyncEnabled);
    CC_SYNTHESIZE_READONLY_NV(bool, m_bControllerConnected, ControllerConnected);
    CC_SYNTHESIZE_NV(bool, m_bSleepMode, SleepMode);
    CC_SYNTHESIZE_NV(bool, m_bForceTimer, ForceTimer);
    CC_SYNTHESIZE_NV(bool, m_bSmoothFix, SmoothFix);
    CC_SYNTHESIZE_NV(bool, m_bFullscreen, Fullscreen);

protected:
    static CCApplication * sm_pSharedApplication;
};

NS_CC_END

#endif    // __CC_APPLICATION_WIN32_H__
