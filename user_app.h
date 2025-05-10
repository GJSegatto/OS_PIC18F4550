#ifndef USER_APP_H
#define	USER_APP_H

#include "types.h"

TASK acelerador(void);
TASK controle_central(void);
TASK injecao_eletronica(void);

void user_config(void);

#endif	/* USER_APP_H */

