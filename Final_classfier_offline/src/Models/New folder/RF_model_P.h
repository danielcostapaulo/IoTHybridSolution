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
                        if (x[10] <= 247.0) {
                            if (x[13] <= 958.0) {
                                if (x[1] <= 514.5) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[15] <= 700.5) {
                                if (x[15] <= 298.5) {
                                    if (x[13] <= 982.0) {
                                        votes[2] += 1;
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[11] <= 213.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }
                        }

                        // tree #2
                        if (x[15] <= 303.5) {
                            if (x[1] <= 448.0) {
                                if (x[13] <= 1892.0) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[15] <= 700.5) {
                                votes[0] += 1;
                            }

                            else {
                                if (x[16] <= 286.0) {
                                    if (x[15] <= 2742.0) {
                                        votes[0] += 1;
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

                        // tree #3
                        if (x[15] <= 319.5) {
                            if (x[5] <= 194.5) {
                                if (x[3] <= 343.5) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                votes[0] += 1;
                            }
                        }

                        else {
                            if (x[13] <= 2982.0) {
                                if (x[5] <= 312.0) {
                                    if (x[6] <= 39.0) {
                                        if (x[5] <= 128.0) {
                                            votes[2] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    votes[1] += 1;
                                }
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #4
                        if (x[11] <= 469.0) {
                            if (x[15] <= 308.0) {
                                if (x[13] <= 1639.0) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[15] <= 800.0) {
                                    votes[0] += 1;
                                }

                                else {
                                    if (x[13] <= 277.5) {
                                        votes[2] += 1;
                                    }

                                    else {
                                        if (x[16] <= 247.0) {
                                            votes[0] += 1;
                                        }

                                        else {
                                            votes[1] += 1;
                                        }
                                    }
                                }
                            }
                        }

                        else {
                            if (x[17] <= 659.5) {
                                votes[0] += 1;
                            }

                            else {
                                votes[1] += 1;
                            }
                        }

                        // tree #5
                        if (x[5] <= 399.5) {
                            if (x[17] <= 162.0) {
                                if (x[6] <= 78.0) {
                                    votes[2] += 1;
                                }

                                else {
                                    votes[0] += 1;
                                }
                            }

                            else {
                                if (x[15] <= 295.5) {
                                    if (x[10] <= 377.0) {
                                        if (x[1] <= 456.0) {
                                            votes[2] += 1;
                                        }

                                        else {
                                            votes[0] += 1;
                                        }
                                    }

                                    else {
                                        votes[0] += 1;
                                    }
                                }

                                else {
                                    if (x[7] <= 1384.0) {
                                        votes[0] += 1;
                                    }

                                    else {
                                        votes[1] += 1;
                                    }
                                }
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
#define PEAK_N 3 
#define USE_STD 0 
#define SORT 1 
