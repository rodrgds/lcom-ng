
int compareScores(const void* a, const void* b) {
    const leaderboardValue* valueA = (const leaderboardValue*)a;
    const leaderboardValue* valueB = (const leaderboardValue*)b;
    
    if (valueA->score < valueB->score) {
        return -1;
    } else if (valueA->score > valueB->score) {
        return 1;
    } else {
        return 0;
    }
}