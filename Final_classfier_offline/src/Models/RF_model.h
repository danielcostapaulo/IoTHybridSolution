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
                        uint8_t votes[3] = { 0 };
                        // tree #1
                        if (x[6] <= 481.7200012207031) {
                            if (x[28] <= 507.760009765625) {
                                if (x[31] <= 57.26500129699707) {
                                    if (x[4] <= 351.5299987792969) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[2] += 1;
                                    }
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[29] <= 52.91000175476074) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }
                        }

                        else {
                            if (x[9] <= 52.35000133514404) {
                                votes[1] += 1;
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        // tree #2
                        if (x[31] <= 75.875) {
                            if (x[5] <= 63.76500129699707) {
                                votes[1] += 1;
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        else {
                            votes[0] += 1;
                        }

                        // tree #3
                        if (x[21] <= 51.13500118255615) {
                            if (x[9] <= 40.935001373291016) {
                                votes[0] += 1;
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // tree #4
                        if (x[11] <= 53.53999996185303) {
                            if (x[15] <= 51.989999771118164) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            votes[2] += 1;
                        }

                        // tree #5
                        if (x[19] <= 56.98500156402588) {
                            if (x[7] <= 40.810001373291016) {
                                votes[0] += 1;
                            }

                            else {
                                votes[2] += 1;
                            }
                        }

                        else {
                            votes[1] += 1;
                        }

                        // return argmax of votes
                        uint8_t classIdx = 0;
                        float maxVotes = votes[0];

                        for (uint8_t i = 1; i < 3; i++) {
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
#define PEAK_N 6 
#define USE_STD 0 
#define SORT 1
