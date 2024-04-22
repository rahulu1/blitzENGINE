/*
 * SceneManager.cpp is part of the eftielEngine source code.
 *
 * Originally developed for use in EECS 498.007
 *
 *
 * MIT License
 *
 * Copyright (c) 2024 Rahul Unniyampath
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "SceneManager.hpp"
#include "ComponentManager.hpp"

void SceneManager::SetCurrentScene(const std::string &scene_to_set)
{
    current_scene = Scene();
    
    if (!dont_destroy_on_load.empty())
    {
        current_scene.actors_by_uuid = dont_destroy_on_load;
        
        for (auto &actor_to_save_pair : dont_destroy_on_load)
        {
            current_scene.uuids_by_name[actor_to_save_pair.second->name].insert(actor_to_save_pair.second->uuid);
            
            actor_to_save_pair.second->AddComponents();
            
            current_scene.AddActorToSceneLifecycleMaps(actor_to_save_pair.second);
        }
    }
    
    current_scene.Init(scene_to_set);
}


Actor* SceneManager::cppActorInstantiate(const std::string &actor_template_name)
{
    std::shared_ptr<Actor> template_actor = std::make_shared<Actor>(actor_templates.try_emplace(actor_template_name, actor_template_name).first->second.template_actor);
    return current_scene.QueueActorToAdd(template_actor).get();
}


void SceneManager::cppSceneDontDestroy(Actor *actor)
{
    std::shared_ptr<Actor> actor_to_save = current_scene.DontDestroy(actor);
    if (actor_to_save.get() != nullptr)
        dont_destroy_on_load.try_emplace(actor_to_save->uuid, actor_to_save);
}
