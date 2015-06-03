#ifndef CONTROLLERFACTORY_H__
#define CONTROLLERFACTORY_H__

#include <vector>
#include <functional>

#include "Enums.h"

class PlayerController;
class Board;

class ControllerFactory
{
public:

    typedef std::function<PlayerController* (std::string, PlayerColor, const Board&)> ConstructorFnType;

    ControllerFactory() {};
    ~ControllerFactory() {};

    int constructor_count() const {return m_constructors.size();}

    int register_constructor(std::string prompt, ConstructorFnType constructor);

    PlayerController* construct(int controller_id, std::string name, PlayerColor color,
            const Board& initial_board) const;

    PlayerController* prompt_user_controller_selection(std::string name, PlayerColor color,
            const Board& initial_board) const;
private:

    std::vector<ConstructorFnType> m_constructors;
    std::vector<std::string> m_prompts;
};


#endif
    
