#include "ControllerFactory.h"

#include <utility>
#include <iostream>

#include "PlayerController.h"

int ControllerFactory::register_constructor(std::string prompt,
        ConstructorFnType constructor)
{
    m_constructors.push_back(constructor);
    m_prompts.push_back(prompt); 

    return m_constructors.size()-1;
}
 
PlayerController* ControllerFactory::construct(int controller_id, std::string name,
        PlayerColor color, const Board& initial_board) const
{
    auto controller = m_constructors[controller_id](std::move(name), color, initial_board);

    controller->set_kind_id(controller_id);
    return controller;
}
 
PlayerController* ControllerFactory::prompt_user_controller_selection(std::string name,
        PlayerColor color, const Board& initial_board) const
{

    while(true) {
        int choice = -1;
        std::cout << "How is " << name << " controlled?\n";
        
        for(int i = 0; i < m_constructors.size(); ++i) {
            std::cout << i+1 << ") " << m_prompts[i] << "\n";
        }
        std::cout << std::endl;
        std::cin >> choice;

        int max_choice = m_constructors.size();
        if(choice < 1 | choice > max_choice) {
            std::cout << "That is not a valid choice" << std::endl;
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        } else {
            return construct(choice-1, std::move(name), color, initial_board);
        }
    }
    return nullptr;
}
 
