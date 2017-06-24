//
//  DieWithError.c
//  Server_ChatApp
//
//  Created by Mayara Brandao Dusheyko on 2/25/17.
//  Copyright © 2017 Mayara Brandao Dusheyko. All rights reserved.
//

#include "global.h"
#include <stdio.h> /* for perror() */
#include <stdlib.h> /* for exit() */

//Error Handling
void DieWithError(char *errorMessage)
{
    perror(errorMessage) ;
    exit(1);
}
