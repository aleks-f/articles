//
// FileServerBundleActivator.cpp
//
// Copyright (c) 2013, Nucor Steel Memphis.
///


#include "Poco/ClassLibrary.h"
#include "Poco/Exception.h"
#include "Poco/OSP/BundleActivator.h"
#include "Poco/OSP/ServiceFinder.h"
#include "Poco/OSP/WebEvent/WebEventService.h"
#include "Poco/Format.h"
#include "Poco/Delegate.h"
#include <iostream>

using namespace Poco;
using namespace Poco::OSP;
using namespace Poco::Util;


namespace FileServer {


class FileServerBundleActivator: public Poco::OSP::BundleActivator
	/// Bundle activator
{
public:
	FileServerBundleActivator(): _pContext(0), _pWebEventService(0), _pNotification(0)
		/// Creates FileServerBundleActivator.
	{
	}

	virtual ~FileServerBundleActivator()
		/// Destroys FileServerBundleActivator.
	{
	}

	void start(BundleContext::Ptr pContext)
		/// Invoked by OSP when bundle is started.
	{
		_pContext = pContext;
		
		log().information("Starting FileServer bundle ...");

		ServiceRef::Ptr pWebEventServiceRef = _pContext->registry().findByName(WebEvent::WebEventService::SERVICE_NAME);
		if (pWebEventServiceRef)
		{
			_pWebEventService = pWebEventServiceRef->castedInstance<WebEvent::WebEventService>();
		}
		else
		{
			log().error("WebEvent Service reference is NULL.");
			throw NullPointerException("WebEvent Service is null.");
		}

		_pNotification = &_pWebEventService->subjectNotified("org.poco.demo");
		if (_pNotification)
			*_pNotification += Poco::delegate(this, &FileServerBundleActivator::onNotify);
		else
		{
			log().error("WebEvent Notification reference is NULL.");
			throw NullPointerException("WebEvent Notification reference is null.");
		}

		log().information("FileServer bundle start completed succesfully.");
	}

	void stop(Poco::OSP::BundleContext::Ptr pContext)
		/// Invoked by OSP when bundle is stopped.
	{
		log().information("Stopping FileServer bundle ...");

		*_pNotification -= Poco::delegate(this, &FileServerBundleActivator::onNotify);

		log().information("FileServer bundle stopped.");
	}

private:
	typedef Poco::OSP::WebEvent::WebEventService                    WebEventSvc;
	typedef Poco::OSP::WebEvent::WebEventService::Ptr               WebEvtSvcPtr;
	typedef Poco::OSP::WebEvent::WebEventService::NotificationEvent WebNotificationEvent;
	typedef Poco::BasicEvent<const WebNotificationEvent>            NotificationEvent;

	Poco::Logger& log() const
		/// Returns reference to the log stream.
	{
		return _pContext->logger();
	}

	void onNotify(const WebNotificationEvent& ev)
	{
		std::cout << ev.second << std::flush;
		if (ev.second == "\r") std::cout << std::endl;
	}
	BundleContext::Ptr _pContext;
	WebEvtSvcPtr       _pWebEventService;
	NotificationEvent* _pNotification;
};


} // namespace Areas


using namespace FileServer;


POCO_BEGIN_MANIFEST(BundleActivator)
	POCO_EXPORT_CLASS(FileServerBundleActivator)
POCO_END_MANIFEST
