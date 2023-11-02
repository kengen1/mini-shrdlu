#ifndef ACTION_h_
#define ACTION_h_

class Action {
    public:
        int source;
        int destination;
        int heuristic;

        Action(){};

        Action(int from, int to, int h){
            source = from;
            destination = to;
            heuristic = h;
        }

        Action(int from, int to){
            source = from;
            destination = to;
        }

        bool operator<(const Action a) const {
            if(heuristic < a.heuristic)
                return true;
            else
            return false;
        }
};

#endif //PLAYER_h
