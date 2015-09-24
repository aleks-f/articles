//
// FileServerRequestHandler.cpp 
//
// $Id: //projects/Nucor/ReportRouter/src/FileServerRequestHandler.cpp#1 $
//
// Copyright (c) 2013, Nucor Steel Memphis.


#include "Poco/LogStream.h"
#include "Poco/StringTokenizer.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/OSP/Configuration.h"
#include "Poco/OSP/BundleContext.h"
#include "Poco/OSP/ServiceFinder.h"
#include "Poco/OSP/PreferencesService.h"
#include "Poco/OSP/Web/WebSession.h"
#include "Poco/OSP/Web/WebRequestHandlerFactory.h"
#include "Poco/OSP/Web/MediaTypeMapper.h"
#include "Poco/ClassLibrary.h"
#include "Poco/NumberFormatter.h"
#include "Poco/URI.h"
#include "Poco/Mutex.h"
#include "Poco/Util/AbstractConfiguration.h"
#include <string>
#include <iostream>


using namespace Poco;
using namespace Poco::Net;
using namespace Poco::OSP;
using namespace Poco::OSP::Web;


class FileServerRequestHandler: public HTTPRequestHandler
{
public:
	FileServerRequestHandler(BundleContext::Ptr pContext,
		const std::string& root, const std::string& defFileName, LogStream& log):
			_pContext(pContext),
			_root(root),
			_defFileName(defFileName),
			_log(log)
	{
	}

	~FileServerRequestHandler()
	{
	}

	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
	{
		_log.information() << "Request from " << request.clientAddress().toString()
			<< ": " << request.getMethod() << " " << request.getURI() << " " << request.getVersion() << std::endl;

		Poco::URI uri(request.getURI());
		std::string path = uri.getPath();
		if (path.find("..") != std::string::npos)
		{
			sendResponse(request, HTTPResponse::HTTP_BAD_REQUEST, request.getURI());
			return;
		}
		Poco::Path requestPath(path, Poco::Path::PATH_UNIX);
		Poco::Path localPath(_root);
		localPath.makeDirectory();
		for (int i = 0; i < requestPath.depth(); i++)
		{
			localPath.pushDirectory(requestPath[i]);
		}
		localPath.append(requestPath.getFileName());
		try
		{
			Poco::File f(localPath);
			if (!f.isDirectory())
			{
				if (f.exists())  // a file from the server explicitly requested
				{
					sendFile(request, response, f.path());
					return;
				}
			}
			else // a directory requested
			{
				std::string::size_type pos = path.size();
				if (pos != std::string::npos && path.at(pos - 1) == '/')
				{
					sendFile(request, response, localPath);
					return;
				}
				else // case of a directory without terminating slash
				{
					path += '/';
					uri.setPath(path);
					response.redirect(uri.toString());
					return;
				}
			}
		}
		catch (Poco::FileNotFoundException&) // file not found, try data I/O request
		{
			std::string fName = localPath.toString();
			Poco::File localFile(fName);
			Poco::File propertiesFile(fName + ".properties");
		}
		sendNotFound(request, path);
	}

private:

	void sendFile(HTTPServerRequest& request,
		HTTPServerResponse& response,
		const Poco::Path& localPath)
	{
		Poco::File f(localPath.toString());
		if (f.isDirectory()) // directory forward to default file
		{
			StringTokenizer tokenizer(_defFileName, ";,", StringTokenizer::TOK_TRIM);
			bool found = false;
			for (StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it)
			{
				Poco::Path indexPath(localPath, *it);
				Poco::File indexFile(indexPath.toString());
				if (indexFile.exists())
				{
					response.sendFile(indexPath.toString(), "text/html");
					found = true;
					break;
				}
			}

			if (!found) throw Poco::NotFoundException(request.getURI());
		}
		else if (f.exists()) // file itself ws found
		{
			Poco::OSP::Web::MediaTypeMapper::Ptr pMTM = ServiceFinder::find<Poco::OSP::Web::MediaTypeMapper>(_pContext);
			response.sendFile(f.path(), pMTM->map(localPath.getExtension()));
		}
		else throw Poco::NotFoundException(request.getURI());
	}

	std::string htmlize(const std::string& str)
	{
		std::string::const_iterator it(str.begin());
		std::string::const_iterator end(str.end());
		std::string html;
		for (; it != end; ++it)
		{
			switch (*it)
			{
			case '<': html += "&lt;"; break;
			case '>': html += "&gt;"; break;
			case '"': html += "&quot;"; break;
			case '&': html += "&amp;"; break;
			default:  html += *it; break;
			}
		}
		return html;
	}

	void sendResponse(HTTPServerRequest& request,
		HTTPResponse::HTTPStatus status,
		const std::string& message)
	{
		const std::string& serverName = request.serverParams().getServerName();
		request.response().setContentType("text/html");
		request.response().setStatusAndReason(status);
		std::string html("<HTML><HEAD><TITLE>");
		html += NumberFormatter::format(static_cast<int>(status));
		html += " - ";
		html += request.response().getReasonForStatus(status);
		html += "</TITLE></HEAD><BODY><H1>";
		html += NumberFormatter::format(static_cast<int>(status));
		html += " - ";
		html += request.response().getReasonForStatus(status);
		html += "</H1><PRE>";
		html += htmlize(message);
		html += "</PRE><HR><ADDRESS>";
		html += htmlize(serverName);
		html += " at ";
		html += request.serverAddress().toString();
		html += "</ADDRESS></BODY></HTML>";
		request.response().sendBuffer(html.data(), html.size());
	}

	void sendNotFound(HTTPServerRequest& request, const std::string& path)
	{
		sendResponse(request, HTTPResponse::HTTP_NOT_FOUND, path);
	}
	
	BundleContext::Ptr _pContext;
	const std::string& _root;
	const std::string& _defFileName;
	LogStream&         _log;
};


class FileServerRequestHandlerFactory: public WebRequestHandlerFactory
{
public:
	FileServerRequestHandlerFactory() : _pLog(0)
	{
	}

	~FileServerRequestHandlerFactory()
	{
		delete _pLog;
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		if (_pContext.isNull()) init();
		return new FileServerRequestHandler(_pContext, _root, _defFileName, *_pLog);
	}

private:
	const Poco::OSP::Configuration& globalConfig(BundleContext::Ptr pContext)
	{
		return *(ServiceFinder::find<OSP::PreferencesService>(pContext)->configuration());
	}

	const Poco::Util::AbstractConfiguration& bundleConfig(BundleContext::Ptr pContext)
	{
		poco_assert (!pContext.isNull());
		return pContext->thisBundle()->properties();
	}

	const Poco::OSP::Configuration& globalConfig()
	{
		return *(ServiceFinder::find<OSP::PreferencesService>(_pContext)->configuration());
	}
	
	const Poco::Util::AbstractConfiguration& bundleConfig()
	{
		poco_assert (!_pContext.isNull());
		return _pContext->thisBundle()->properties();
	}

	void init()
	{
		_pContext = context();

		// web server root, never give anything above this directory level
		const Poco::Util::AbstractConfiguration& bdlCfg = bundleConfig(_pContext);
		const Poco::Util::AbstractConfiguration& glbCfg = globalConfig(_pContext);
		_root = bdlCfg.getString("http.webserver.root", glbCfg.getString("http.webserver.root", bdlCfg.getString("http.webserver.root")));
		_root = glbCfg.expand(_root);
		// default file name, substituted when no file name specified in request
		_defFileName = bundleConfig(_pContext).getString("http.webserver.filename.default", 
			globalConfig(_pContext).getString("http.webserver.filename.default", "index.html"));

		_pLog = new LogStream(_pContext->logger());
	}

	BundleContext::Ptr _pContext;
	std::string        _root;
	std::string        _defFileName;
	LogStream*         _pLog;
};


POCO_BEGIN_NAMED_MANIFEST(WebServer, WebRequestHandlerFactory)
	POCO_EXPORT_CLASS(FileServerRequestHandlerFactory)
POCO_END_MANIFEST
