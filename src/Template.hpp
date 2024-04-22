//
//  Template.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 1/31/24.
//

#ifndef Template_hpp
#define Template_hpp

#define TEMPLATES_PATH "resources/actor_templates/"

#include "Actor.hpp"

#include <filesystem>
#include <string>


namespace fs = std::filesystem;

class Template {
public:
    
    
    Template(const std::string &template_name);
    
    
    Actor template_actor;
};


inline Template::Template(const std::string &template_name)
{
    std::string template_path = TEMPLATES_PATH + template_name + ".template";
    if(!fs::exists(template_path))
        ErrorExit("error: template " + template_name + " is missing");
    
    rapidjson::Document template_doc;
    ReadJsonFile(template_path, template_doc);
    
    template_actor.UpdateActorWithJSON(template_doc);
}

#endif /* Template_hpp */
