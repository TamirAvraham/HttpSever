#include "HttpServer.h"

//#include <regex>
#include <sstream>

//std::vector<std::string> regex_split(const std::string& input, const std::regex& pattern)
//{
//    std::vector<std::string> result;
//    std::copy(std::sregex_token_iterator(input.begin(), input.end(), pattern, -1),
//        std::sregex_token_iterator(),
//        std::back_inserter(result));
//    return result;
//}
//
//std::vector<http::HttpRouteParam> GetParams(const std::string& route, const std::string& routeTemplate)
//{
//    std::vector<http::HttpRouteParam> params;
//
//    // Split the route string into individual components
//    auto routeComponents = regex_split(route, std::regex("/"));
//
//    // Split the route template string into individual components
//    auto routeTemplateComponents = regex_split(routeTemplate, std::regex("/"));
//
//    // Iterate over both route components and template components
//    for (size_t i = 0; i < routeComponents.size(); i++)
//    {
//        // If the current component in the route template starts with a colon, it's a parameter
//        if (!routeTemplateComponents[i].empty() && routeTemplateComponents[i][0] == ':')
//        {
//            // Extract the parameter name
//            auto paramName = routeTemplateComponents[i].substr(1);
//
//            // Add the parameter name and value to the params vector
//            params.emplace_back(http::HttpRouteParam(paramName, routeComponents[i]));
//        }
//    }
//
//    return params;
//}

std::pair<bool, std::vector<http::HttpRouteParam>> http::HttpServer::matchRoute(std::string route, std::string templateRoute) const
{
    std::stringstream routeAsStream(route),templateRouteAsStream(templateRoute);
    std::vector<std::string> routeVector, templateRouteVector;

    auto getRouteComponents = [](std::vector<std::string>& vec,std::stringstream& routeToSplit) {
        std::string part;
        while (std::getline(routeToSplit, part, '/'))
        {
            vec.push_back(part);
        }
    };

    getRouteComponents(routeVector, routeAsStream);
    getRouteComponents(templateRouteVector, templateRouteAsStream);

    if (routeVector.size()!=templateRouteVector.size())
    {
        return{ false,{} };
    }

    std::vector<http::HttpRouteParam> params;

    for (size_t i = 0; i < templateRouteVector.size(); i++)
    {
        if (templateRouteVector[i][0] == ':')
        {
            std::string cache1 = templateRouteVector[i].substr(1), cache2 = routeVector[i];
            if (cache1 == cache2)
                params.push_back({ cache1,cache2 });
            else
                return{ false,{} };
            continue;
        }
        else if (templateRouteVector[i] != routeVector[i])
            return { false,{} };
    }

    return {true,params};
}
//
//std::vector<http::HttpRouteParam> http::HttpServer::getRouteParams(std::string route, std::string parttern) const
//{
//    return GetParams(route,parttern);
//}
