#pragma once
#include <cstdarg>
namespace Eloquent {
    namespace ML {
        namespace Port {
            class RandomForest {
                public:
                    /**
                    * Predict class for features vector
                    */
                    int predict(float *x) {
                        uint8_t votes[9] = { 0 };
                        // tree #1
                        if (x[16] <= 755.1300048828125) {
                            if (x[13] <= 7048.5) {
                                if (x[1] <= 19368.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[3] += 1;
                                }
                            }

                            else {
                                if (x[15] <= 21651.0) {
                                    if (x[13] <= 41701.0) {
                                        if (x[13] <= 11173.0) {
                                            if (x[1] <= 119520.0) {
                                                votes[5] += 1;
                                            }

                                            else {
                                                votes[8] += 1;
                                            }
                                        }

                                        else {
                                            if (x[17] <= 3474.0) {
                                                if (x[13] <= 18808.5) {
                                                    votes[8] += 1;
                                                }

                                                else {
                                                    if (x[0] <= 65.10000228881836) {
                                                        votes[8] += 1;
                                                    }

                                                    else {
                                                        if (x[7] <= 2872.125) {
                                                            votes[8] += 1;
                                                        }

                                                        else {
                                                            votes[6] += 1;
                                                        }
                                                    }
                                                }
                                            }

                                            else {
                                                if (x[13] <= 20934.0) {
                                                    if (x[17] <= 16935.5) {
                                                        if (x[1] <= 211440.0) {
                                                            votes[8] += 1;
                                                        }

                                                        else {
                                                            if (x[15] <= 11437.0) {
                                                                votes[6] += 1;
                                                            }

                                                            else {
                                                                votes[8] += 1;
                                                            }
                                                        }
                                                    }

                                                    else {
                                                        if (x[11] <= 2113.125) {
                                                            votes[6] += 1;
                                                        }

                                                        else {
                                                            votes[5] += 1;
                                                        }
                                                    }
                                                }

                                                else {
                                                    if (x[15] <= 12242.0) {
                                                        votes[6] += 1;
                                                    }

                                                    else {
                                                        if (x[5] <= 141800.0) {
                                                            votes[7] += 1;
                                                        }

                                                        else {
                                                            votes[8] += 1;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    else {
                                        votes[4] += 1;
                                    }
                                }

                                else {
                                    if (x[9] <= 46658.0) {
                                        votes[2] += 1;
                                    }

                                    else {
                                        if (x[10] <= 781.1750183105469) {
                                            if (x[15] <= 34372.0) {
                                                votes[2] += 1;
                                            }

                                            else {
                                                votes[4] += 1;
                                            }
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[2] <= 299.4499969482422) {
                                if (x[12] <= 117.17499923706055) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[4] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #2
                        if (x[15] <= 92620.0) {
                            if (x[7] <= 609.9349975585938) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[13] <= 6772.25) {
                                    votes[3] += 1;
                                }

                                else {
                                    if (x[15] <= 21651.0) {
                                        if (x[7] <= 26764.5) {
                                            if (x[0] <= 117.18000030517578) {
                                                if (x[3] <= 111544.0) {
                                                    if (x[9] <= 1643.75) {
                                                        if (x[7] <= 2947.0) {
                                                            votes[8] += 1;
                                                        }

                                                        else {
                                                            votes[6] += 1;
                                                        }
                                                    }

                                                    else {
                                                        if (x[8] <= 781.1749877929688) {
                                                            if (x[10] <= 624.9400024414062) {
                                                                votes[8] += 1;
                                                            }

                                                            else {
                                                                votes[6] += 1;
                                                            }
                                                        }

                                                        else {
                                                            votes[8] += 1;
                                                        }
                                                    }
                                                }

                                                else {
                                                    if (x[5] <= 130600.0) {
                                                        if (x[10] <= 637.9599914550781) {
                                                            if (x[15] <= 19735.0) {
                                                                votes[8] += 1;
                                                            }

                                                            else {
                                                                votes[5] += 1;
                                                            }
                                                        }

                                                        else {
                                                            if (x[1] <= 117656.0) {
                                                                votes[5] += 1;
                                                            }

                                                            else {
                                                                if (x[3] <= 150704.0) {
                                                                    votes[8] += 1;
                                                                }

                                                                else {
                                                                    votes[6] += 1;
                                                                }
                                                            }
                                                        }
                                                    }

                                                    else {
                                                        if (x[1] <= 189128.0) {
                                                            votes[8] += 1;
                                                        }

                                                        else {
                                                            votes[7] += 1;
                                                        }
                                                    }
                                                }
                                            }

                                            else {
                                                votes[5] += 1;
                                            }
                                        }

                                        else {
                                            votes[4] += 1;
                                        }
                                    }

                                    else {
                                        if (x[9] <= 49774.0) {
                                            votes[2] += 1;
                                        }

                                        else {
                                            votes[4] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[12] <= 130.19499969482422) {
                                votes[4] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #3
                        if (x[13] <= 7036.25) {
                            if (x[5] <= 30756.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[3] += 1;
                            }
                        }

                        else {
                            if (x[3] <= 190336.0) {
                                if (x[16] <= 455.6800079345703) {
                                    if (x[16] <= 390.5849914550781) {
                                        if (x[6] <= 117.18000030517578) {
                                            if (x[5] <= 43855.5) {
                                                if (x[7] <= 2929.25) {
                                                    if (x[16] <= 155.73500061035156) {
                                                        votes[8] += 1;
                                                    }

                                                    else {
                                                        votes[5] += 1;
                                                    }
                                                }

                                                else {
                                                    if (x[17] <= 20643.0) {
                                                        if (x[12] <= 130.19499969482422) {
                                                            votes[6] += 1;
                                                        }

                                                        else {
                                                            votes[4] += 1;
                                                        }
                                                    }

                                                    else {
                                                        votes[2] += 1;
                                                    }
                                                }
                                            }

                                            else {
                                                if (x[11] <= 2191.625) {
                                                    votes[8] += 1;
                                                }

                                                else {
                                                    if (x[3] <= 150624.0) {
                                                        if (x[15] <= 16024.0) {
                                                            votes[7] += 1;
                                                        }

                                                        else {
                                                            votes[8] += 1;
                                                        }
                                                    }

                                                    else {
                                                        votes[8] += 1;
                                                    }
                                                }
                                            }
                                        }

                                        else {
                                            if (x[14] <= 221.3300018310547) {
                                                if (x[16] <= 299.4499969482422) {
                                                    if (x[8] <= 338.50999450683594) {
                                                        votes[4] += 1;
                                                    }

                                                    else {
                                                        votes[2] += 1;
                                                    }
                                                }

                                                else {
                                                    votes[8] += 1;
                                                }
                                            }

                                            else {
                                                votes[5] += 1;
                                            }
                                        }
                                    }

                                    else {
                                        if (x[7] <= 16900.0) {
                                            if (x[16] <= 429.63999938964844) {
                                                votes[8] += 1;
                                            }

                                            else {
                                                votes[6] += 1;
                                            }
                                        }

                                        else {
                                            votes[4] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[13] <= 21604.0) {
                                        votes[5] += 1;
                                    }

                                    else {
                                        if (x[1] <= 405344.0) {
                                            votes[2] += 1;
                                        }

                                        else {
                                            votes[4] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[8] <= 703.0550231933594) {
                                    if (x[10] <= 781.1750183105469) {
                                        if (x[7] <= 41074.0) {
                                            if (x[13] <= 16774.0) {
                                                votes[5] += 1;
                                            }

                                            else {
                                                votes[2] += 1;
                                            }
                                        }

                                        else {
                                            votes[4] += 1;
                                        }
                                    }

                                    else {
                                        if (x[7] <= 49408.0) {
                                            votes[5] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #4
                        if (x[11] <= 47900.0) {
                            if (x[7] <= 609.9349975585938) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[7] <= 16700.0) {
                                    if (x[15] <= 7188.75) {
                                        if (x[13] <= 7588.25) {
                                            votes[3] += 1;
                                        }

                                        else {
                                            if (x[16] <= 181.77499389648438) {
                                                if (x[7] <= 2898.625) {
                                                    votes[8] += 1;
                                                }

                                                else {
                                                    votes[6] += 1;
                                                }
                                            }

                                            else {
                                                votes[5] += 1;
                                            }
                                        }
                                    }

                                    else {
                                        if (x[0] <= 117.18000030517578) {
                                            if (x[2] <= 429.63999938964844) {
                                                if (x[16] <= 377.56500244140625) {
                                                    if (x[3] <= 179184.0) {
                                                        if (x[1] <= 200120.0) {
                                                            if (x[16] <= 325.49000549316406) {
                                                                votes[8] += 1;
                                                            }

                                                            else {
                                                                if (x[9] <= 1423.375) {
                                                                    votes[6] += 1;
                                                                }

                                                                else {
                                                                    votes[5] += 1;
                                                                }
                                                            }
                                                        }

                                                        else {
                                                            if (x[3] <= 111208.0) {
                                                                if (x[13] <= 19545.5) {
                                                                    votes[8] += 1;
                                                                }

                                                                else {
                                                                    votes[6] += 1;
                                                                }
                                                            }

                                                            else {
                                                                votes[7] += 1;
                                                            }
                                                        }
                                                    }

                                                    else {
                                                        votes[6] += 1;
                                                    }
                                                }

                                                else {
                                                    votes[8] += 1;
                                                }
                                            }

                                            else {
                                                votes[6] += 1;
                                            }
                                        }

                                        else {
                                            votes[5] += 1;
                                        }
                                    }
                                }

                                else {
                                    if (x[9] <= 50522.0) {
                                        votes[2] += 1;
                                    }

                                    else {
                                        votes[4] += 1;
                                    }
                                }
                            }
                        }

                        else {
                            if (x[16] <= 663.9950256347656) {
                                if (x[5] <= 220096.0) {
                                    votes[4] += 1;
                                }

                                else {
                                    if (x[15] <= 83160.0) {
                                        votes[4] += 1;
                                    }

                                    else {
                                        if (x[4] <= 859.2850189208984) {
                                            votes[4] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }

                            else {
                                if (x[4] <= 233.85000610351562) {
                                    votes[4] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #5
                        if (x[9] <= 712.0599975585938) {
                            if (x[7] <= 1529.0599975585938) {
                                votes[0] += 1;
                            }

                            else {
                                votes[8] += 1;
                            }
                        }

                        else {
                            if (x[0] <= 39.060001373291016) {
                                if (x[5] <= 111744.0) {
                                    votes[3] += 1;
                                }

                                else {
                                    if (x[1] <= 14824.0) {
                                        votes[8] += 1;
                                    }

                                    else {
                                        votes[4] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[15] <= 92816.0) {
                                    if (x[7] <= 44056.0) {
                                        if (x[1] <= 236536.0) {
                                            if (x[4] <= 442.6600036621094) {
                                                if (x[1] <= 119824.0) {
                                                    if (x[13] <= 7080.75) {
                                                        votes[3] += 1;
                                                    }

                                                    else {
                                                        votes[5] += 1;
                                                    }
                                                }

                                                else {
                                                    if (x[7] <= 2909.375) {
                                                        if (x[7] <= 2550.25) {
                                                            if (x[9] <= 2414.875) {
                                                                if (x[3] <= 150632.0) {
                                                                    if (x[10] <= 481.7200012207031) {
                                                                        votes[8] += 1;
                                                                    }

                                                                    else {
                                                                        votes[6] += 1;
                                                                    }
                                                                }

                                                                else {
                                                                    votes[6] += 1;
                                                                }
                                                            }

                                                            else {
                                                                votes[7] += 1;
                                                            }
                                                        }

                                                        else {
                                                            votes[8] += 1;
                                                        }
                                                    }

                                                    else {
                                                        if (x[17] <= 10340.0) {
                                                            votes[6] += 1;
                                                        }

                                                        else {
                                                            if (x[15] <= 15171.0) {
                                                                if (x[17] <= 11830.0) {
                                                                    votes[8] += 1;
                                                                }

                                                                else {
                                                                    votes[6] += 1;
                                                                }
                                                            }

                                                            else {
                                                                votes[8] += 1;
                                                            }
                                                        }
                                                    }
                                                }
                                            }

                                            else {
                                                votes[5] += 1;
                                            }
                                        }

                                        else {
                                            votes[2] += 1;
                                        }
                                    }

                                    else {
                                        votes[4] += 1;
                                    }
                                }

                                else {
                                    if (x[0] <= 130.19499969482422) {
                                        votes[4] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }
                        }

                        // return argmax of votes
                        uint8_t classIdx = 0;
                        float maxVotes = votes[0];

                        for (uint8_t i = 1; i < 9; i++) {
                            if (votes[i] > maxVotes) {
                                classIdx = i;
                                maxVotes = votes[i];
                            }
                        }

                        return classIdx;
                    }

                protected:
                };
            }
        }
    }
#define USE_PEAK 1 
#define PEAK_N 3 
#define USE_STD 0 
#define SORT 1 
