/// Input manager

struct Input
{
    /// listener interface

    struct Listener
    {
        virtual void pressed(Key key) = 0;
    };

    /// test listener

    struct TestListener : public Listener
    {
        void pressed(Key key)
        {
            switch (key)
            {
                case Left: printf("left key pressed\n"); break;
                case Right: printf("right key pressed\n"); break;
                case Up: printf("up key pressed\n"); break;
                case Down: printf("down key pressed\n"); break;
                case Space: printf("space key pressed\n"); break;
                case Enter: printf("enter key pressed\n"); break;
                case Control: printf("control key pressed\n"); break;
                case Esc: printf("escape key pressed\n"); break;
                case PageUp: printf("page up key pressed\n"); break;
                case PageDown: printf("page down key pressed\n"); break;
                case F1: printf("f1 key pressed\n"); break;
                case F2: printf("f2 key pressed\n"); break;
                case F3: printf("f3 key pressed\n"); break;
                case F4: printf("f4 key pressed\n"); break;
                case F5: printf("f5 key pressed\n"); break;
                case F6: printf("f6 key pressed\n"); break;
                case F7: printf("f7 key pressed\n"); break;
                case F8: printf("f8 key pressed\n"); break;
                case F9: printf("f9 key pressed\n"); break;
            }
        }
    };

    /// constructor

    Input()
    {
        memset(this, 0, sizeof(Input));

        //static TestListener testListener;
        //listener = &testListener;

        listener = 0;
        time = 0;

        #ifdef LOGGING
        logfile = fopen("input.log", "w");
        #endif
    }

    /// destructor

    ~Input()
    {
        #ifdef LOGGING
        fprintf(logfile, "%d: quit\n", time);
        fclose(logfile);
        logfile = 0;
        #endif
    }

    /// update

    void update(unsigned int t)
    {
        time = t;

        if (current!=previous)
        {
            // detect key presses

            if (listener)
            {
                if (current.left && !previous.left)
                    listener->pressed(Left);

                if (current.right && !previous.right)
                    listener->pressed(Right);

                if (current.up && !previous.up)
                    listener->pressed(Up);

                if (current.down && !previous.down)
                    listener->pressed(Down);

                if (current.space && !previous.space)
                    listener->pressed(Space);

                if (current.enter && !previous.enter)
                    listener->pressed(Enter);

                if (current.control && !previous.control)
                    listener->pressed(Control);

                if (current.escape && !previous.escape)
                    listener->pressed(Esc);

                if (current.pageUp && !previous.pageUp)
                    listener->pressed(PageUp);

                if (current.pageDown && !previous.pageDown)
                    listener->pressed(PageDown);

                if (current.f1 && !previous.f1)
                    listener->pressed(F1);

                if (current.f2 && !previous.f2)
                    listener->pressed(F2);

                if (current.f3 && !previous.f3)
                    listener->pressed(F3);

                if (current.f4 && !previous.f4)
                    listener->pressed(F4);

                if (current.f5 && !previous.f5)
                    listener->pressed(F5);

                if (current.f6 && !previous.f6)
                    listener->pressed(F6);

                if (current.f7 && !previous.f7)
                    listener->pressed(F7);

                if (current.f8 && !previous.f8)
                    listener->pressed(F8);

                if (current.f9 && !previous.f9)
                    listener->pressed(F9);
            }

            // set to previous

            previous = current;

            // log to "input.log" to enable playback later

            #ifdef LOGGING
            fprintf( logfile, "%d: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
                     t,
                     current.left, 
                     current.right, 
                     current.up, 
                     current.down, 
                     current.space, 
                     current.enter, 
                     current.control, 
                     current.escape, 
                     current.pageUp, 
                     current.pageDown, 
                     current.f1, 
                     current.f2, 
                     current.f3, 
                     current.f4, 
                     current.f5, 
                     current.f6, 
                     current.f7, 
                     current.f8,
                     current.f9 );
            #endif
        }

        time++;
    }

    void onKeyDown(Key key)
    {
        switch (key)
        {
            case Left:
                current.left = true;
                break;

            case Right:
                current.right = true;
                break;

            case Up:
                current.up = true;
                break;

            case Down:
                current.down = true;
                break;

            case Space:
                current.space = true;
                break;

            case Enter:
                current.enter = true;
                break;

            case Control:
                current.control = true;
                break;

            case Esc:
                current.escape = true;
                break;

            case PageUp:
                current.pageUp = true;
                break;

            case PageDown:
                current.pageDown = true;
                break;

            case F1:
                current.f1 = true;
                break;

            case F2:
                current.f2 = true;
                break;

            case F3:
                current.f3 = true;
                break;

            case F4:
                current.f4 = true;
                break;

            case F5:
                current.f5 = true;
                break;

            case F6:
                current.f6 = true;
                break;

            case F7:
                current.f7 = true;
                break;

            case F8:
                current.f8 = true;
                break;

            case F9:
                current.f9 = true;
                break;
        }
    }

    void onKeyUp(Key key)
    {
        switch (key)
        {
            case Left:
                current.left = false;
                break;

            case Right:
                current.right = false;
                break;

            case Up:
                current.up = false;
                break;

            case Down:
                current.down = false;
                break;

            case Space:
                current.space = false;
                break;

            case Enter:
                current.enter = false;
                break;

            case Control:
                current.control = false;
                break;

            case Esc:
                current.escape = false;
                break;

            case PageUp:
                current.pageUp = false;
                break;

            case PageDown:
                current.pageDown = false;
                break;

            case F1:
                current.f1 = false;
                break;

            case F2:
                current.f2 = false;
                break;

            case F3:
                current.f3 = false;
                break;

            case F4:
                current.f4 = false;
                break;

            case F5:
                current.f5 = false;
                break;

            case F6:
                current.f6 = false;
                break;

            case F7:
                current.f7 = false;
                break;

            case F8:
                current.f8 = false;
                break;

            case F9:
                current.f9 = false;
                break;
        }
    }

    /// attach listener

    void attach(Listener *listener)
    {
        this->listener = listener;          // just one for now...
    }

    // current input value accessors

    bool left() const
    {
        return current.left;
    }

    bool right() const
    {
        return current.right;
    }

    bool up() const
    {
        return current.up;
    }

    bool down() const
    {
        return current.down;
    }

    bool space() const
    {
        return current.space;
    }

    bool enter() const
    {
        return current.enter;
    }

    bool control() const
    {
        return current.control;
    }

    bool escape() const
    {
        return current.escape;
    }

    bool f1() const
    {
        return current.f1;
    }

    bool pageUp() const
    {
        return current.pageUp;
    }

    bool pageDown() const
    {
        return current.pageDown;
    }

    bool f2() const
    {
        return current.f2;
    }

    bool f3() const
    {
        return current.f3;
    }

    bool f4() const
    {
        return current.f4;
    }

    bool f5() const
    {
        return current.f5;
    }

    bool f6() const
    {
        return current.f6;
    }

    bool f7() const
    {
        return current.f7;
    }

    bool f8() const
    {
        return current.f8;
    }

    bool f9() const
    {
        return current.f9;
    }

    Listener *listener;             ///< if non-null this object is notified of key presses.

private: 

    struct Data
    {
        bool left;
        bool right;
        bool up;
        bool down;
        bool space;
        bool enter;
        bool control;
        bool escape;
        bool pageUp;
        bool pageDown;
        bool f1;
        bool f2;
        bool f3;
        bool f4;
        bool f5;
        bool f6;
        bool f7;
        bool f8;
        bool f9;

        bool operator==(const Data &other) const
        {
            return left==other.left && 
                   right==other.right &&
                   up==other.up &&
                   down==other.down &&
                   space==other.space &&
                   enter==other.enter &&
                   control==other.control &&
                   escape==other.escape &&
                   pageUp==other.pageUp && 
                   pageDown==other.pageDown && 
                   f1==other.f1 && 
                   f2==other.f2 && 
                   f3==other.f3 && 
                   f4==other.f4 && 
                   f5==other.f5 && 
                   f6==other.f6 && 
                   f7==other.f7 && 
                   f8==other.f8 &&
                   f9==other.f9;
        }

        bool operator!=(const Data &other) const
        {
            return !(*this==other);
        }
    };

    Data current;
    Data previous;

    FILE *logfile;

    unsigned int time;
};
