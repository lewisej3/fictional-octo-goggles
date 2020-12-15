#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h> 
#include <Poco/Net/HTTPSessionFactory.h>
#include <Poco/Net/HTTPCredentials.h> 
#include <Poco/Net/HTMLForm.h>
#include <Poco/StreamCopier.h>
#include <Poco/NullStream.h> 
#include <Poco/Path.h> 
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <iostream> 
 
 
using Poco::Net::HTTPClientSession; 
using Poco::Net::HTTPRequest; 
using Poco::Net::HTTPResponse; 
using Poco::Net::HTTPMessage; 
using Poco::StreamCopier; 
using Poco::Path; 
using Poco::URI; 
using Poco::Exception; 
 
 
bool doRequest(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, Poco::Net::HTTPResponse& response) 
{ 
    session.sendRequest(request);
    std::istream& rs = session.receiveResponse(response); 
    std::cout << response.getStatus() << " " << response.getReason() << std::endl; 
    if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED) 
    { 
        StreamCopier::copyStream(rs, std::cout); 
        return true; 
    }
    else
    {
        Poco::NullOutputStream null;
        StreamCopier::copyStream(rs, null);
        return false;
    }
}


int main(int argc, char** argv)
{
    try
    {
		Poco::URI uri("http://0.0.0.0//post");
		Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
        session.setKeepAlive(true);
		HTTPRequest postRequest(HTTPRequest::HTTP_POST, uri.getPathAndQuery(), HTTPMessage::HTTP_1_1);
        HTTPResponse response;
		std::string reqBody("Thisisatest=testing&Another=test2");
		postRequest.setContentLength(reqBody.length());
		postRequest.setKeepAlive(true);

		std::ostream& myOStream = session.sendRequest(postRequest);
		myOStream << reqBody;

		postRequest.write(std::cout);

		std::istream& responseStream = session.receiveResponse(response);
		std::cerr << responseStream.rdbuf();
		std::stringstream rawJson;
		Poco::StreamCopier::copyStream(responseStream, rawJson);
		Poco::JSON::Parser parser;
		auto result_ = parser.parse(rawJson);
		//auto jsonObject = result_.extract<Poco::JSON::Object::Ptr>();
		std::ostringstream ostr;
		//Poco::JSON::Stringifier::stringify(jsonObject, ostr, 4);
		std::cout << ostr.rdbuf();

    }
    catch (Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        return 1;
    }
    return 0;
}
   

