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
                        if (x[2] <= 221.3300018310547) {
                            if (x[13] <= 89384.0) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        else {
                            if (x[1] <= 2719.625) {
                                if (x[4] <= 168.7550048828125) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #2
                        if (x[3] <= 648.6899995803833) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[1] <= 2217.0) {
                                if (x[13] <= 284992.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #3
                        if (x[13] <= 44104.0) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[5] <= 3642.25) {
                                if (x[3] <= 1557.875) {
                                    if (x[4] <= 325.49000549316406) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        votes[2] += 1;
                                    }
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #4
                        if (x[11] <= 864.4749984741211) {
                            if (x[7] <= 1299.9199981689453) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[13] <= 259856.0) {
                                    votes[1] += 1;
                                }

                                else {
                                    votes[2] += 1;
                                }
                            }
                        }

                        else {
                            if (x[14] <= 51.58000183105469) {
                                if (x[1] <= 2498.75) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                if (x[1] <= 2014.0) {
                                    if (x[2] <= 182.2750015258789) {
                                        votes[1] += 1;
                                    }

                                    else {
                                        if (x[8] <= 169.25499725341797) {
                                            votes[1] += 1;
                                        }

                                        else {
                                            votes[2] += 1;
                                        }
                                    }
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #5
                        if (x[9] <= 977.670000076294) {
                            votes[0] += 1;
                        }

                        else {
                            if (x[14] <= 51.58000183105469) {
                                if (x[2] <= 260.38999938964844) {
                                    votes[2] += 1;
                                }

                                else {
                                    if (x[11] <= 4242.75) {
                                        votes[2] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
                            }

                            else {
                                if (x[1] <= 2298.75) {
                                    if (x[14] <= 169.25) {
                                        votes[2] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
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
#define PEAK_N 3 
#define USE_STD 0 
#define SORT 1 
