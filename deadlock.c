#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_RESOURCES 100
#define MAX_PROCESSES 100

typedef struct {
    int id;
    int site;
    int heldBy; // Process ID of the process holding this resource, -1 if not held
} Resource;

typedef struct {
    int id;
    Resource *holding;
    Resource *waiting;
} Process;

bool detectCycle(Process *processes, Resource *resources, int numProcesses, Process *cur, int start) {
    if (cur -> holding == NULL)
	return false;

    for (int i = 0; i < numProcesses; i++) {
        if (cur -> waiting != NULL && processes[i].holding != NULL &&
            cur -> waiting -> id == processes[i].holding -> id) {
            if (processes[i].id == start || detectCycle(processes, resources, numProcesses, &processes[i], start))
                return true;
        }
    }
    return false;
}

bool checkDeadlockSite(Process *processes, Resource *resources, int site, int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        if (processes[i].id != -1 && processes[i].holding != NULL && processes[i].waiting != NULL &&
            processes[i].holding -> site == site && processes[i].waiting -> site == site &&
            detectCycle(processes, resources, numProcesses, &processes[i], processes[i].id))
            return true;
    }
    return false;
}

bool checkDeadlock(Process *processes, Resource *resources, int numProcesses) {
    for (int i = 0; i < numProcesses; i++) {
        if (processes[i].waiting != NULL &&
            detectCycle(processes, resources, numProcesses, &processes[i], processes[i].id)) {
            bool waitingFromDifferentSite = false;
            for (int j = 0; j < numProcesses; j++) {
                if (processes[j].id != -1 && processes[j].holding != NULL && processes[j].waiting != NULL &&
                    processes[j].holding -> site != processes[j].waiting -> site) {
                    waitingFromDifferentSite = true;
                    break;
                }
            }
            return waitingFromDifferentSite;
        }
    }
    return false;
}

int main() {
    Resource resources[MAX_RESOURCES] = {0};
    Process processes[MAX_PROCESSES] = {{-1, NULL, NULL}};

    int numSites, numResources, numProcesses;
    int resourceIdCounter = 0;

    printf("\nEnter the number of sites: ");
    scanf("%d", &numSites);
    printf("Enter the number of resources: ");
    scanf("%d", &numResources);
    printf("Enter the number of processes: ");
    scanf("%d", &numProcesses);

    printf("\n");

    for (int s = 1; s <= numSites; s++) {
        int numSiteResources;
        printf("Enter the number of resources in site %d: ", s);
        scanf("%d", &numSiteResources);

        for (int i = 0; i < numSiteResources; i++, resourceIdCounter++) {
            resources[resourceIdCounter].id = resourceIdCounter;
            resources[resourceIdCounter].site = s;
	    resources[resourceIdCounter].heldBy = -1;
        }
    }

    printf("\n");

    for (int i = 0; i < numProcesses; i++) {
        int site, hld, wai;
        printf("\nEnter the site for process-%d: ", i);
        scanf("%d", &site);
	printf("Enter the holding resource for process-%d: ", i);
        scanf("%d", &hld);
	printf("Enter the waiting resource for process-%d: ", i);
        scanf("%d", &wai);

	processes[i].id = i;

	if (hld != -1) {
            processes[i].holding = &resources[hld];
            resources[hld].heldBy = i;
        } else
            processes[i].holding = NULL;

        if (wai != -1) {
            processes[i].waiting = &resources[wai];
        } else
            processes[i].waiting = NULL;
    }

    for (int s = 1; s <= numSites; s++) {
        if (checkDeadlockSite(processes, resources, s, numProcesses)) {
            printf("Deadlock detected in site %d\n", s);
        }
    }

    bool globalDeadlock = checkDeadlock(processes, resources, numProcesses);

    if (globalDeadlock)
	printf("\nDeadlock detected in the central coordinator\n");
    else
	printf("\nNo deadlock detected\n");

    return 0;
}
