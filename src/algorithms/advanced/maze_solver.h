#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <Arduino.h>
#include "junction_detector.h"

class MazeSolver {
private:
    static const int MAX_PATH_LENGTH = 100;
    char path[MAX_PATH_LENGTH];
    int pathLength;
    
    void simplifyPath() {
        // Simplify sequences like "LBR" to "S"
        for (int i = 0; i < pathLength - 2; i++) {
            if (path[i] == 'L' && path[i+1] == 'B' && path[i+2] == 'R') {
                path[i] = 'S';
                for (int j = i+1; j < pathLength-2; j++) {
                    path[j] = path[j+2];
                }
                pathLength -= 2;
            }
        }
    }

public:
    MazeSolver() : pathLength(0) {}

    void reset() {
        pathLength = 0;
    }

    void addTurn(char turn) {
        if (pathLength < MAX_PATH_LENGTH) {
            path[pathLength++] = turn;
            simplifyPath();
        }
    }

    // Left-hand rule maze solving
    char decideTurn(JunctionType junction) {
        switch (junction) {
            case LEFT_TURN:
                addTurn('L');
                return 'L';
            
            case RIGHT_TURN:
                addTurn('R');
                return 'R';
            
            case T_JUNCTION:
                addTurn('L');  // Always take left turn in T-junction
                return 'L';
            
            case CROSS_JUNCTION:
                addTurn('L');  // Always take left turn in cross junction
                return 'L';
            
            case END_OF_LINE:
                addTurn('B');  // Turn back
                return 'B';
            
            default:
                return 'S';  // Straight
        }
    }

    const char* getPath() {
        return path;
    }

    int getPathLength() {
        return pathLength;
    }
};

#endif 