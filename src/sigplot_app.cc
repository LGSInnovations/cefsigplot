// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "sigplot_app.h"

#include <string>
#include <iostream>
#include <libgen.h>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "sigplot_handler.h"


namespace {

// When using the Views framework this object provides the delegate
// implementation for the CefWindow that hosts the Views-based browser.
class SigPlotWindowDelegate : public CefWindowDelegate {
 public:
  explicit SigPlotWindowDelegate(CefRefPtr<CefBrowserView> browser_view)
      : browser_view_(browser_view) {}

  void OnWindowCreated(CefRefPtr<CefWindow> window) OVERRIDE {
    // Add the browser view and show the window.
    window->AddChildView(browser_view_);
    window->Show();

    // Give keyboard focus to the browser view.
    browser_view_->RequestFocus();
  }

  void OnWindowDestroyed(CefRefPtr<CefWindow> window) OVERRIDE {
    browser_view_ = NULL;
  }

  bool CanClose(CefRefPtr<CefWindow> window) OVERRIDE {
    // Allow the window to close if the browser says it's OK.
    CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
    if (browser)
      return browser->GetHost()->TryCloseBrowser();
    return true;
  }

 private:
  CefRefPtr<CefBrowserView> browser_view_;

  IMPLEMENT_REFCOUNTING(SigPlotWindowDelegate);
  DISALLOW_COPY_AND_ASSIGN(SigPlotWindowDelegate);
};

}  // namespace

SigPlotApp::SigPlotApp() {}

void SigPlotApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) {
  command_line->AppendSwitch("disable-web-security");
}

void SigPlotApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  CefRefPtr<CefCommandLine> command_line =
      CefCommandLine::GetGlobalCommandLine();

#if defined(OS_WIN) || defined(OS_LINUX)
  // Create the browser using the Views framework if "--use-views" is specified
  // via the command-line. Otherwise, create the browser using the native
  // platform framework. The Views framework is currently only supported on
  // Windows and Linux.
  const bool use_views = command_line->HasSwitch("use-views");
#else
  const bool use_views = false;
#endif

  // SigPlotHandler implements browser-level callbacks.
  CefRefPtr<SigPlotHandler> handler(new SigPlotHandler(use_views));

  // Specify CEF browser settings here.
  CefBrowserSettings browser_settings;

  std::string url;

  // Get the program path
  std::string prog = command_line->GetProgram().ToString();
  // KLUDGE TO FIND FOLDER RELATIVE TO APP, MAKE THIS MORE PORTABLE
  // PERHAPS WITH BOOST
  size_t found = prog.find_last_of("/\\");
  std::string mydir = prog.substr(0, found);
  std::string sigplot = mydir + "/app/app.html";
  std::string full_path = realpath(sigplot.c_str(), NULL);
  url = "file://" + full_path;

  if (use_views) {
    // Create the BrowserView.
    CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
        handler, url, browser_settings, NULL, NULL);

    // Create the Window. It will show itself after creation.
    CefWindow::CreateTopLevelWindow(new SigPlotWindowDelegate(browser_view));

  } else {
    // Information used when creating the native window.
    CefWindowInfo window_info;

#if defined(OS_WIN)
    // On Windows we need to specify certain flags that will be passed to
    // CreateWindowEx().
    window_info.SetAsPopup(NULL, "cefsimple");
#endif

    // Create the first browser window.
    CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,
                                  NULL);
  }
}
