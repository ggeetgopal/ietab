#pragma once

#include <atlbase.h>
#include <atlwin.h>
#include <ExDispid.h>

#include "CustomClientSite.h"

class CPlugin;

class /*ATL_NO_VTABLE*/ CWebBrowser : public CComPtr<IWebBrowser2>,
					public CWindowImpl<CWebBrowser, CAxWindow>,
					public IDispEventImpl<0, CWebBrowser> {
public:
	CWebBrowser(CPlugin* plugin);
	~CWebBrowser();

	HWND Create(HWND parentWnd, RECT& rc) {
		// we can also use about:blank instead of Shell.Explorer here
		HWND ret = CWindowImpl<CWebBrowser, CAxWindow>::Create(parentWnd, 
																	&rc, 
																	_T("about:blank"),
																	WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL);
		return ret;
	}

	bool GetCanBack() {
		return m_CanBack;
	}

	bool GetCanForward() {
		return m_CanForward;
	}

	void SetPlugin(CPlugin* plugin) {
		m_Plugin = plugin;
	}

	CPlugin* GetPlugin() {
		return m_Plugin;
	}

private:
	BEGIN_MSG_MAP(CWebBrowser)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		// MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		// MESSAGE_HANDLER(WM_PARENTNOTIFY, OnParentNotify)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam , LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam , LPARAM lParam, BOOL& bHandled);
	// LRESULT OnParentNotify(UINT uMsg, WPARAM wParam , LPARAM lParam, BOOL& bHandled);

	// override this to do cleanup
	virtual void OnFinalMessage(HWND hwnd);

		// WindowProc for innermost Internet_Explorer_Server window.
	static LRESULT InnerWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK GetMsgHookProc(int code, WPARAM wParam, LPARAM lParam);

	BEGIN_SINK_MAP(CWebBrowser)
		SINK_ENTRY(0, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
		SINK_ENTRY(0, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
		SINK_ENTRY(0, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2)
		SINK_ENTRY(0, DISPID_NEWWINDOW2, OnNewWindow2)
		SINK_ENTRY(0, DISPID_PROGRESSCHANGE, OnProgressChange)
		SINK_ENTRY(0, DISPID_SETSECURELOCKICON, OnSetSecureLockIcon)
		SINK_ENTRY(0, DISPID_STATUSTEXTCHANGE, OnStatusTextChange)
		SINK_ENTRY(0, DISPID_TITLECHANGE, OnTitleChange)
		SINK_ENTRY(0, DISPID_WINDOWCLOSING, OnWindowClosing)
		SINK_ENTRY(0, DISPID_COMMANDSTATECHANGE, OnCommandStateChange )
	END_SINK_MAP()

	void __stdcall OnNewWindow2(IDispatch **ppDisp, VARIANT_BOOL *Cancel);
	void __stdcall OnBeforeNavigate2(IDispatch *pDisp, VARIANT *url, VARIANT *Flags, VARIANT *TargetFrameName, VARIANT *PostData, VARIANT *Headers, VARIANT_BOOL *Cancel);
	void __stdcall OnNavigateComplete2(IDispatch* pDisp,  VARIANT* URL);
	void __stdcall OnDocumentComplete(IDispatch *pDisp, VARIANT *URL);
	void __stdcall OnProgressChange(long Progress, long ProgressMax);
	void __stdcall OnSetSecureLockIcon(long SecureLockIcon);
	void __stdcall OnStatusTextChange(BSTR Text);
	void __stdcall OnTitleChange(BSTR Text);
	void __stdcall OnWindowClosing(VARIANT_BOOL IsChildWindow, VARIANT_BOOL *Cancel);
	void __stdcall OnCommandStateChange(long Command, VARIANT_BOOL Enable);

private:
	CPlugin* m_Plugin;
	HWND m_hInnerWnd;
	WNDPROC m_OldInnerWndProc;
	CComQIPtr<IOleInPlaceActiveObject> m_pInPlaceActiveObject;
	bool m_CanBack;
	bool m_CanForward;

	CComObject<CCustomClientSite>* m_ClientSite;

	static int browserCount;
	static ATOM winPropAtom;
	static HHOOK getMsgHook;
};
