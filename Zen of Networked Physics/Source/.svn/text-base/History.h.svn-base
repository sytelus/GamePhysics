/// History buffer.
/// Stores a history of all "moves" (time, input, state) since the last 
/// correction received from the server, plus a list of all *important moves*
/// (changes in input) in the same time period.
/// Used in client side prediction to apply server corrections 'in the past'
/// Press F4 while running to toggle visualization of the history buffer.

class History
{
public:

    History(int size = 1000)
    {
        moves.resize(size);
        importantMoves.resize(size);

        #ifdef LOGGING
        logfile = fopen("history.log", "w");
        #endif
    }

    ~History()
    {
        #ifdef LOGGING
        if (logfile)
        {
            fclose(logfile);
            logfile = 0;
        }
        #endif
    }

    void add(const Move &move)
    {
        // log for comparison

        if (logfile)
        {
            Vector position = move.state.position;
            Quaternion orientation = move.state.orientation;
            Cube::Input input = move.input;
            fprintf(logfile, "%d: position=(%f,%f,%f), orientation=(%f,%f,%f,%f), input=(%d,%d,%d,%d,%d)\n", move.time, position.x, position.y, position.z, orientation.w, orientation.x, orientation.y, orientation.z, input.left, input.right, input.forward, input.back, input.jump);
        }

        // determine if important move

        bool important = true;

        if (!moves.empty())
        {
            const Move &previous = moves.newest();

            important = move.input.left!=previous.input.left || 
                        move.input.right!=previous.input.right ||
                        move.input.forward!=previous.input.forward ||
                        move.input.back!=previous.input.back ||
                        move.input.jump!=previous.input.jump;
        }

        if (important)
            importantMoves.add(move);

        // add move to history

        moves.add(move);
    }

    void correction(Scene &scene, unsigned int t, const Cube::State &state, const Cube::Input &input)
    {
        // discard out of date important moves 

        while (importantMoves.oldest().time<t && !importantMoves.empty())
            importantMoves.remove();

        // discard out of date moves

        while (moves.oldest().time<t && !moves.empty())
            moves.remove();
        
        if (moves.empty())
            return;

        // compare correction state with move history state

        if (state!=moves.oldest().state)
        {
            // discard corrected move

            moves.remove();

            // save current scene data

            unsigned int savedTime = scene.time;
            Cube::Input savedInput = scene.input;

            // rewind to correction and replay moves

            scene.time = t;
            scene.input = input;
            scene.cube.snap(state);

            scene.replaying = true;

            int i = moves.tail;

            while (i!=moves.head)
            {
                while (scene.time<moves[i].time)
                    scene.update(scene.time);
                scene.input = moves[i].input;
                moves[i].state = scene.cube.state();
                moves.next(i);
            }

            scene.update(scene.time);
            
            scene.replaying = false;

            // restore saved input

            scene.input = savedInput;
        }
    }

    /// render history buffer as a cool trail

    void render()
    {
        int i = moves.tail;

        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        Cube::State previous;

        int count = 0;

        while (i!=moves.head)
        {
            const Cube::State &state = moves[i].state;

            if (count++==0)
            {
                previous = state;
                continue;
            }

            Vector a = state.bodyToWorld * (Vector(-1,-1,-1) * state.size * 0.5);
            Vector b = state.bodyToWorld * (Vector(+1,-1,-1) * state.size * 0.5);
            Vector c = state.bodyToWorld * (Vector(+1,+1,-1) * state.size * 0.5);
            Vector d = state.bodyToWorld * (Vector(-1,+1,-1) * state.size * 0.5);
            Vector e = state.bodyToWorld * (Vector(-1,-1,+1) * state.size * 0.5);
            Vector f = state.bodyToWorld * (Vector(+1,-1,+1) * state.size * 0.5);
            Vector g = state.bodyToWorld * (Vector(+1,+1,+1) * state.size * 0.5);
            Vector h = state.bodyToWorld * (Vector(-1,+1,+1) * state.size * 0.5);

            Vector _a = previous.bodyToWorld * (Vector(-1,-1,-1) * previous.size * 0.5);
            Vector _b = previous.bodyToWorld * (Vector(+1,-1,-1) * previous.size * 0.5);
            Vector _c = previous.bodyToWorld * (Vector(+1,+1,-1) * previous.size * 0.5);
            Vector _d = previous.bodyToWorld * (Vector(-1,+1,-1) * previous.size * 0.5);
            Vector _e = previous.bodyToWorld * (Vector(-1,-1,+1) * previous.size * 0.5);
            Vector _f = previous.bodyToWorld * (Vector(+1,-1,+1) * previous.size * 0.5);
            Vector _g = previous.bodyToWorld * (Vector(+1,+1,+1) * previous.size * 0.5);
            Vector _h = previous.bodyToWorld * (Vector(-1,+1,+1) * previous.size * 0.5);
            
            previous = state;

            glBegin(GL_QUADS);
                
                glColor3f(0.02f, 0.02f, 0.02f);

                quad(a, b, _b, _a);
                quad(b, c, _c, _b);
                quad(c, d, _d, _c);
                quad(d, a, _a, _d);

                quad(e, f, _f, _e);
                quad(f, g, _g, _f);
                quad(g, h, _h, _g);
                quad(h, e, _e, _h);

                quad(a, e, _e, _a);
                quad(b, f, _f, _b);
                quad(c, g, _g, _c);
                quad(d, h, _h, _d);

            glEnd();

            moves.next(i);
        }

        glDisable(GL_BLEND);

        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
    }

    /// get important moves in a std::vector form

    void importantMoveArray(std::vector<Move> &array)
    {
        const int size = importantMoves.size();

        array.resize(size);

        int i = importantMoves.tail;

        for (int j=0; j<size; j++)
        {
            array[j] = importantMoves[i];
            importantMoves.next(i);
        }
    }

private:

    /// circular buffer class

    struct CircularBuffer
    {
        int head;
        int tail;

        CircularBuffer()
        {
            head = 0;
            tail = 0;
        }

        void resize(int size)
        {
            head = 0;
            tail = 0;
            moves.resize(size);
        }

        int size()
        {
            int count = head - tail;
            if (count<0)
                count += (int) moves.size();
            return count;
        }

        void add(const Move &move)
        {
            moves[head] = move;
            next(head);

        }

        void remove()
        {
            assert(!empty());
            next(tail);
        }

        Move& oldest()
        {
            assert(!empty());
            return moves[tail];
        }

        Move& newest()
        {
            assert(!empty());
            int index = head-1;
            if (index==-1)
                index = (int) moves.size() - 1;
            return moves[index];
        }

        bool empty() const
        {
            return head==tail;
        }

        void next(int &index)
        {
            index ++;
            if (index>=(int)moves.size()) 
                index -= (int)moves.size();
        }

        void previous(int &index)
        {
            index --;
            if (index<0)
                index += (int)moves.size();
        }

        Move& operator[](int index)
        {
            assert(index>=0);
            assert(index<(int)moves.size());
            return moves[index];
        }

    private:

        std::vector<Move> moves;
    };

private:

    CircularBuffer moves;                       ///< stores all recent moves
    CircularBuffer importantMoves;              ///< stores recent *important* moves

    FILE *logfile;
};
