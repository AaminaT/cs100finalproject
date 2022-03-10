#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "../Observer/listener.hpp"
#include "../Messages/update.hpp"
#include "board.hpp"
#include "piece.hpp"
#include <stack>
#include <unordered_map>

class Game: public Observer, public Listener {
    private:
	    int id;
        Board* current;
        std::stack<Board*> history;
        std::unordered_map<char, Piece*> pieces;

        void handle_move(Move* mov, Observer* src) {
            auto ptm = current->at(mov->get_origin());
            Piece* ruleset = nullptr;

            if(ptm.hasPiece()) 
                ruleset = pieces.at(ptm.piece());

            if(ruleset != nullptr && (current->get_turn()%2 == 0) == ptm.isWhite() && ruleset->isMoveValid(mov, current)) {
                // apply move
                history.push(current);
                current = current->move(mov->get_origin(), mov->get_destination(), 200);
                Message* upd = new UpdateBoard(current);
                notifyObservers(upd, nullptr);
            }
            else {
                // notify user of invalid move
                // Message* inv = new InvalidMove(*mov);
                // src->update(inv, nullptr);
            }
        }

        void handle_undo() {
            if(current->get_turn() == 0) {
                // notify user of invalid move
                // Message* inv = new InvalidMove(*mov);
                // src->update(inv, nullptr);
            }
            else {
                delete current;
                current = history.top();
                history.pop();
            }
        }

        /*virtual void handle(Promotion* pro, Observer* src) {

        }*/

    public:
	    Game(Observer* player1, Observer* player2): Observer(), Listener() {
            this->addObserver(player1);
            this->addObserver(player2);
        }
        
        ~Game() {
            for(auto p: pieces)
                delete p.second;
            while(!history.empty()) {
                delete history.top();
                history.pop();
            }
        }

        virtual void update(Message* msg, Observer* src) {
            switch(msg->m_type()) {
                case 'm':
                    handle_move(dynamic_cast<Move*>(msg), src);
                    break;
                case 'u':
                    handle_undo();
                    break;
                case 'p':
                    break;
                default:
            }
        }

        virtual void notifyObservers(Message* msg, Observer* src) {
            observers->at(current->get_turn()%2)->update(msg, src);
        }

        
};

#endif //__GAME_HPP__
