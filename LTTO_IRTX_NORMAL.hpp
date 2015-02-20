#include "LTTO_IRTX.hpp"

#include "../pinstate/pinstate.hpp"

#ifndef LTTO_IRTX_NORMAL_H
#define LTTO_IRTX_NORMAL_H

class LTTO_IRTX_NORMAL : public LTTO_IRTX{
    public:
        LTTO_IRTX_NORMAL( void (*IRLEDON)(void), void (*IRLEDOFF)(void));
    private:
        void (*LEDON)(void);
        void (*LEDOFF)(void);
        
        void update_IRLED(ePIN_STATE state);
};

#endif
