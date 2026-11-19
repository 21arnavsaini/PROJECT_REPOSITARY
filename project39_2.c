
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEAMS 4
#define MATCHES 4
#define VENUES 3
#define INF 999999

/* ---------------- LEAGUE DATA ---------------- */

typedef struct {
    char name[20];
    int points;
    int goalDifference;
} Team;

typedef struct {
    int id;
    int start;
    int end;
    int revenue;
    int cost;
} Fixture;

Team teams[TEAMS] = {
    {"Mumbai", 18, 9},
    {"Delhi", 22, 14},
    {"Bengaluru", 22, 18},
    {"Chennai", 15, 4}
};

Fixture fixtures[MATCHES] = {
    {1, 14, 16, 60, 10},
    {2, 15, 17, 100, 20},
    {3, 16, 18, 140, 30},
    {4, 18, 20, 125, 25}
};

int venueAvailability[3][3] = {
    {1, 1, 0},
    {1, 0, 0},
    {0, 1, 1}
};

int networkDistance[4][4] = {
    {0, 10, 25, 30},
    {10, 0, 15, 20},
    {25, 15, 0, 12},
    {30, 20, 12, 0}
};

int travelDistance[4][4] = {
    {0, 12, 29, 22},
    {12, 0, 19, 25},
    {29, 19, 0, 15},
    {22, 25, 15, 0}
};

char *cityNames[4] = {
    "Mumbai", "Delhi", "Bengaluru", "Chennai"
};

/* ---------------- DISPLAY FUNCTIONS ---------------- */

void printLine() {
    printf("------------------------------------------------------------\n");
}

void header() {
    printf("\n");
    printf("============================================================\n");
    printf("              SPORTS LEAGUE MANAGEMENT SYSTEM\n");
    printf("============================================================\n");
    printf("                       SEASON 2026-27\n");
    printf("============================================================\n");
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
}

void displayMenu() {
    printf("\n");
    printLine();
    printf("                    LEAGUE CONTROL PANEL\n");
    printLine();

    printf("  1. League Dashboard\n");
    printf("  2. Team Standings\n");
    printf("  3. Fixture Schedule\n");
    printf("  4. Venue Allocation\n");
    printf("  5. Broadcast Network\n");
    printf("  6. Broadcast Budget\n");
    printf("  7. Travel Routes\n");
    printf("  8. Away Tour Planner\n");
    printf("  9. Tournament Calendar\n");
    printf(" 10. Player Fatigue Report\n");
    printf("  0. Exit System\n");

    printLine();
    printf("Enter your choice: ");
}

/* ---------------- TEAM STANDINGS ---------------- */

void merge(Team arr[], int left, int mid, int right) {
    Team temp[TEAMS];

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        if (arr[i].points > arr[j].points ||
            (arr[i].points == arr[j].points &&
             arr[i].goalDifference > arr[j].goalDifference)) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid)
        temp[k++] = arr[i++];

    while (j <= right)
        temp[k++] = arr[j++];

    for (i = left; i <= right; i++)
        arr[i] = temp[i];
}

void mergeSort(Team arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void showStandings() {
    Team sortedTeams[TEAMS];

    memcpy(sortedTeams, teams, sizeof(teams));

    mergeSort(sortedTeams, 0, TEAMS - 1);

    printf("\n                  OFFICIAL LEAGUE STANDINGS\n");
    printLine();

    printf("+------+-------------+--------+----------------+\n");
    printf("| Rank | Team        | Points | Goal Difference|\n");
    printf("+------+-------------+--------+----------------+\n");

    for (int i = 0; i < TEAMS; i++) {
        printf("| %-4d | %-11s | %-6d | %+14d |\n",
               i + 1,
               sortedTeams[i].name,
               sortedTeams[i].points,
               sortedTeams[i].goalDifference);
    }

    printf("+------+-------------+--------+----------------+\n");

    printf("\nCurrent League Leader: %s\n",
           sortedTeams[0].name);
}

/* ---------------- FIXTURE SCHEDULING ---------------- */

void sortFixturesByEnd(Fixture arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].end > arr[j + 1].end) {
                Fixture temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int generateFixtures() {
    Fixture temp[MATCHES];

    memcpy(temp, fixtures, sizeof(fixtures));

    sortFixturesByEnd(temp, MATCHES);

    int lastEnd = -1;
    int selected = 0;

    printf("\n                  CONFIRMED MATCH SCHEDULE\n");
    printLine();

    printf("+-----------+-------------+-------------+\n");
    printf("| Match ID  | Start Time  | End Time    |\n");
    printf("+-----------+-------------+-------------+\n");

    for (int i = 0; i < MATCHES; i++) {
        if (temp[i].start >= lastEnd) {
            printf("| %-9d | %-11d | %-11d |\n",
                   temp[i].id,
                   temp[i].start,
                   temp[i].end);

            lastEnd = temp[i].end;
            selected++;
        }
    }

    printf("+-----------+-------------+-------------+\n");

    printf("\nTotal Confirmed TV Slots: %d\n", selected);

    return selected;
}

/* ---------------- VENUE ALLOCATION ---------------- */

int venueMatch[VENUES];

int findVenue(int match, int visited[]) {
    for (int venue = 0; venue < VENUES; venue++) {
        if (venueAvailability[match][venue] &&
            !visited[venue]) {

            visited[venue] = 1;

            if (venueMatch[venue] == -1 ||
                findVenue(venueMatch[venue], visited)) {

                venueMatch[venue] = match;
                return 1;
            }
        }
    }

    return 0;
}

void allocateVenues() {
    for (int i = 0; i < VENUES; i++)
        venueMatch[i] = -1;

    int assigned = 0;

    for (int match = 0; match < VENUES; match++) {
        int visited[VENUES] = {0};

        if (findVenue(match, visited))
            assigned++;
    }

    printf("\n                  STADIUM ALLOCATION REPORT\n");
    printLine();

    printf("+-------------+----------------+\n");
    printf("| Stadium     | Assigned Match |\n");
    printf("+-------------+----------------+\n");

    for (int i = 0; i < VENUES; i++) {
        if (venueMatch[i] != -1) {
            printf("| Stadium %-3d | Match %-9d |\n",
                   i + 1,
                   venueMatch[i] + 1);
        } else {
            printf("| Stadium %-3d | Available      |\n", i + 1);
        }
    }

    printf("+-------------+----------------+\n");

    printf("\nVenues Successfully Allocated: %d/%d\n",
           assigned, VENUES);
}

/* ---------------- BROADCAST NETWORK ---------------- */

void buildBroadcastNetwork() {
    int visited[4] = {0};
    int totalCost = 0;

    visited[0] = 1;

    printf("\n                  BROADCAST NETWORK PLAN\n");
    printLine();

    printf("+-------------+-------------+----------+\n");
    printf("| From City   | To City     | Distance |\n");
    printf("+-------------+-------------+----------+\n");

    for (int count = 0; count < TEAMS - 1; count++) {
        int min = INF;
        int x = -1;
        int y = -1;

        for (int i = 0; i < TEAMS; i++) {
            if (visited[i]) {
                for (int j = 0; j < TEAMS; j++) {
                    if (!visited[j] &&
                        networkDistance[i][j] < min) {

                        min = networkDistance[i][j];
                        x = i;
                        y = j;
                    }
                }
            }
        }

        if (x == -1) {
            printf("\nNetwork cannot connect all cities.\n");
            return;
        }

        printf("| %-11s | %-11s | %-8d |\n",
               cityNames[x], cityNames[y], min);

        visited[y] = 1;
        totalCost += min;
    }

    printf("+-------------+-------------+----------+\n");

    printf("\nTotal Network Distance: %d Units\n", totalCost);
}

/* ---------------- BROADCAST BUDGET ---------------- */

void planBroadcastBudget() {
    int budget = 45;

    int dp[MATCHES + 1][46] = {0};

    for (int i = 1; i <= MATCHES; i++) {
        for (int w = 0; w <= budget; w++) {
            dp[i][w] = dp[i - 1][w];

            if (fixtures[i - 1].cost <= w) {
                int value = fixtures[i - 1].revenue +
                    dp[i - 1][w - fixtures[i - 1].cost];

                if (value > dp[i][w])
                    dp[i][w] = value;
            }
        }
    }

    printf("\n                  BROADCAST BUDGET REPORT\n");
    printLine();

    printf("Available Budget      : %d Units\n", budget);
    printf("Maximum Expected Revenue: %d Units\n",
           dp[MATCHES][budget]);

    printf("\nSelected Broadcast Packages:\n");

    int w = budget;

    for (int i = MATCHES; i > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            printf("Match %d  | Cost: %d | Revenue: %d\n",
                   fixtures[i - 1].id,
                   fixtures[i - 1].cost,
                   fixtures[i - 1].revenue);

            w -= fixtures[i - 1].cost;
        }
    }
}

/* ---------------- TRAVEL ROUTES ---------------- */

void showTravelRoutes() {
    int dist[4][4];

    for (int i = 0; i < TEAMS; i++) {
        for (int j = 0; j < TEAMS; j++) {
            dist[i][j] = travelDistance[i][j];
        }
    }

    for (int k = 0; k < TEAMS; k++) {
        for (int i = 0; i < TEAMS; i++) {
            for (int j = 0; j < TEAMS; j++) {
                if (dist[i][k] != INF &&
                    dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {

                    dist[i][j] =
                        dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printf("\n                  CITY TRAVEL DISTANCES\n");
    printLine();

    printf("%-13s", "City");

    for (int i = 0; i < TEAMS; i++)
        printf("%-12s", cityNames[i]);

    printf("\n");

    for (int i = 0; i < TEAMS; i++) {
        printf("%-13s", cityNames[i]);

        for (int j = 0; j < TEAMS; j++) {
            printf("%-12d", dist[i][j]);
        }

        printf("\n");
    }
}

/* ---------------- AWAY TOUR PLANNER ---------------- */

int bestTour = INF;
int currentPath[4];
int bestPath[4];
int tourVisited[4];

void findBestTour(int city, int count, int cost) {
    if (cost >= bestTour)
        return;

    if (count == TEAMS) {
        int total = cost + travelDistance[city][0];

        if (total < bestTour) {
            bestTour = total;

            for (int i = 0; i < TEAMS; i++)
                bestPath[i] = currentPath[i];
        }

        return;
    }

    for (int next = 0; next < TEAMS; next++) {
        if (!tourVisited[next]) {
            tourVisited[next] = 1;
            currentPath[count] = next;

            findBestTour(
                next,
                count + 1,
                cost + travelDistance[city][next]
            );

            tourVisited[next] = 0;
        }
    }
}

void planAwayTour() {
    bestTour = INF;

    for (int i = 0; i < TEAMS; i++)
        tourVisited[i] = 0;

    tourVisited[0] = 1;
    currentPath[0] = 0;

    findBestTour(0, 1, 0);

    printf("\n                  AWAY TOUR ITINERARY\n");
    printLine();

    printf("Travel Route:\n");

    for (int i = 0; i < TEAMS; i++) {
        printf("%s", cityNames[bestPath[i]]);

        if (i < TEAMS - 1)
            printf(" -> ");
    }

    printf(" -> %s\n", cityNames[0]);

    printf("\nTotal Tour Distance: %d Units\n", bestTour);
}

/* ---------------- TOURNAMENT CALENDAR ---------------- */

int gcdExtended(int a, int b, int *x, int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }

    int x1, y1;

    int gcd = gcdExtended(b % a, a, &x1, &y1);

    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

int modularInverse(int a, int m) {
    int x, y;

    int gcd = gcdExtended(a, m, &x, &y);

    if (gcd != 1)
        return -1;

    return (x % m + m) % m;
}

void synchronizeCalendar() {
    int remainder[3] = {1, 3, 2};
    int modulus[3] = {3, 5, 7};

    int product = 1;

    for (int i = 0; i < 3; i++)
        product *= modulus[i];

    int result = 0;

    for (int i = 0; i < 3; i++) {
        int partial = product / modulus[i];

        int inverse = modularInverse(
            partial % modulus[i],
            modulus[i]
        );

        if (inverse == -1) {
            printf("\nCalendar synchronization failed.\n");
            return;
        }

        result += remainder[i] * partial * inverse;
    }

    result = (result % product + product) % product;

    printf("\n                  TOURNAMENT CALENDAR\n");
    printLine();

    printf("Calendar Cycle 1 : %d\n", remainder[0]);
    printf("Calendar Cycle 2 : %d\n", remainder[1]);
    printf("Calendar Cycle 3 : %d\n", remainder[2]);

    printf("\nCommon Synchronization Day: %d\n", result);
    printf("Combined Calendar Cycle  : %d Days\n", product);
}

/* ---------------- PLAYER FATIGUE REPORT ---------------- */

void showFatigueReport() {
    int n = 500000;

    char pattern[] = "AAAAAA";
    int m = (int)strlen(pattern);

    char *text = malloc((size_t)n + 1);

    if (text == NULL) {
        printf("\nUnable to generate fatigue report.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        if (i % 7 == 0)
            text[i] = 'H';
        else
            text[i] = 'A';
    }

    text[n] = '\0';

    int *lps = malloc((size_t)m * sizeof(int));

    if (lps == NULL) {
        free(text);
        printf("\nUnable to generate fatigue report.\n");
        return;
    }

    int len = 0;
    lps[0] = 0;

    for (int i = 1; i < m;) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else if (len != 0) {
            len = lps[len - 1];
        } else {
            lps[i] = 0;
            i++;
        }
    }

    int count = 0;
    int i = 0;
    int j = 0;

    clock_t startTime = clock();

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }

        if (j == m) {
            count++;
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }

    clock_t endTime = clock();

    double elapsed =
        (double)(endTime - startTime) / CLOCKS_PER_SEC;

    printf("\n                  PLAYER FATIGUE REPORT\n");
    printLine();

    printf("Activity Records Analysed : %d\n", n);
    printf("Activity Pattern Checked  : %s\n", pattern);
    printf("Matching Patterns Found   : %d\n", count);
    printf("Processing Time            : %.6f Seconds\n",
           elapsed);

    printf("\nReport Status: Analysis Completed\n");

    free(text);
    free(lps);
}

/* ---------------- LEAGUE DASHBOARD ---------------- */

void showDashboard() {
    Team sortedTeams[TEAMS];

    memcpy(sortedTeams, teams, sizeof(teams));
    mergeSort(sortedTeams, 0, TEAMS - 1);

    printf("\n                  SEASON OVERVIEW\n");
    printLine();

    printf("Participating Teams       : %d\n", TEAMS);
    printf("Registered Fixtures       : %d\n", MATCHES);
    printf("Available Stadiums        : %d\n", VENUES);
    printf("Current League Leader     : %s\n",
           sortedTeams[0].name);

    printf("\n                  CURRENT STANDINGS\n");
    printLine();

    printf("+------+-------------+--------+----------------+\n");
    printf("| Rank | Team        | Points | Goal Difference|\n");
    printf("+------+-------------+--------+----------------+\n");

    for (int i = 0; i < TEAMS; i++) {
        printf("| %-4d | %-11s | %-6d | %+14d |\n",
               i + 1,
               sortedTeams[i].name,
               sortedTeams[i].points,
               sortedTeams[i].goalDifference);
    }

    printf("+------+-------------+--------+----------------+\n");

    printf("\nLeague Status: Active\n");
}

/* ---------------- MAIN PROGRAM ---------------- */

int main() {
    int choice;

    header();

    do {
        displayMenu();

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input. Please enter a number.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}

            continue;
        }

        switch (choice) {
            case 1:
                showDashboard();
                break;

            case 2:
                showStandings();
                break;

            case 3:
                generateFixtures();
                break;

            case 4:
                allocateVenues();
                break;

            case 5:
                buildBroadcastNetwork();
                break;

            case 6:
                planBroadcastBudget();
                break;

            case 7:
                showTravelRoutes();
                break;

            case 8:
                planAwayTour();
                break;

            case 9:
                synchronizeCalendar();
                break;

            case 10:
                showFatigueReport();
                break;

            case 0:
                printf("\n============================================================\n");
                printf("       THANK YOU FOR USING THE LEAGUE MANAGEMENT SYSTEM\n");
                printf("============================================================\n");
                break;

            default:
                printf("\nInvalid choice. Please select a valid option.\n");
        }

        if (choice != 0) {
            printf("\n");
            pauseScreen();
        }

    } while (choice != 0);

    return 0;
}
