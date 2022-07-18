#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../constant.h"
#include "../data_structures/linked_list.h"
#include "../util.h"

long* getSystemStat(FILE* fp);

void getAllProcData(ListHead* head);

void procListItem_print(ListHead* head);