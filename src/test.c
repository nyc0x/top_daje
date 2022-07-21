#include "./parser/parser.h"
#include "util.h"
#include "./data_structures/proc_list_item.h"

 
/*
    descr:  Call this function in order to test the system stat crawling functionality.  
    args:   None
    retval: None
    author: [MZ] 
*/

void testSysStat(){
    FILE* fp = fopen(SYS_STATS_PATH, "r");

    if(!fp){
        printf("Error while opening /proc/stat file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    long* sys_stats_values = getSystemStat(fp);
    
    /*
        TODO: add more tests.
    */
    //TESTING
    for(int i = 0 ; i <  NUM_SYS_STATS; i++)
        printf("%s : %d \n",SYS_STATS_HEADERS[i],(int) sys_stats_values[i]);
    
    free(sys_stats_values);
    //CLEAN-UP
    fclose(fp);
    return;
}

/*
    descr:  Test function for "countDir(...)" giving current working directory as a path. 
    args:   None
    retval: None
    author: [MZ] 
*/
void testCountDir(){
    //TODO: add more tests.
    printf("[TEST]: I found %ld directories in this path : %s \n",countDir("./"),"./" );
    return;
}

/*
    descr:  Test function which initialises a ListHead item, retrieves all processes' data
            and then prints them in stdout
    args:   None
    retval: None
    author: [NDP] 
*/
void testFillStatList(){
    ListHead head;    
    List_init(&head);
    getAllProcData(&head);
    procListItem_print(&head);
    return;
}


/*ProcListItem* middle(ListHead* head){

    ProcListItem* slow = (ProcListItem* ) head->first;
    ProcListItem* fast = (ProcListItem* ) head->first->next;
 
    while (!slow->list.next && (!fast && !fast->list.next)) {
        slow = (ProcListItem* ) slow->list.next;
        fast = (ProcListItem* ) fast->list.next->next;
    }
    return slow;
}


ListHead* MergeSortList(ListHead* head){

   if (head->first->next == NULL) {
        return head;
    }
 
    ProcListItem* mid = (ProcListItem*)malloc(sizeof(ProcListItem));
    ListHead* head2 = (ListHead*)malloc(sizeof(ListHead));
    
    mid = middle(head);
    head2 = mid->next;
    mid->next = NULL;
    // recursive call to sort() hence diving our problem,
    // and then merging the solution
    Node* finalhead = merge(sort(head), sort(head2));
    return finalhead;
}*/


void testListSorting(){
    ListHead head;    
    List_init(&head);
    getAllProcData(&head);

    /*ListHead * sorted ;
    sorted = MergeSortList(&head);
    */
    procListItem_print(&head);
    

    return;
}


/*
    CMD: make test
*/
int main(int argc, char const *argv[]){
    
    printf("Start testing !\n");

    //testSysStat();
    //testCountDir();
    //testFillStatList();

    testListSorting();

    printf("End testing !\n");
    
    return 0;
}
