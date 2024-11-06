#include <stdint.h>

static const uint8_t piece_data_collision_coords[7][4][4][2] = 
{
    // I PIECE:
    {
        // ROTATION 0:
        {
            { 1, 1 },
            { 2, 1 },
            { 3, 1 },
            { 4, 1 }
        },
        // ROTATION 1:
        {
            { 2, 0 },
            { 2, 1 },
            { 2, 2 },
            { 2, 3 }
        },
        // ROTATION 2:
        {
            { 0, 2 },
            { 1, 2 },
            { 2, 2 },
            { 3, 2 }
        },
        // ROTATION 3:
        {
            { 1, 0 },
            { 1, 1 },
            { 1, 2 },
            { 1, 3 }
        }
    },
    // O PIECE:
    {
        // ROTATION 0:
        {
            { 1, 1 },
            { 2, 1 },
            { 1, 2 },
            { 2, 2 }
        },
        // ROTATION 1:
        {
            { 1, 1 },
            { 2, 1 },
            { 1, 2 },
            { 2, 2 }
        },
        // ROTATION 2:
        {
            { 1, 1 },
            { 2, 1 },
            { 1, 2 },
            { 2, 2 }
        },
        // ROTATION 3:
        {
            { 1, 1 },
            { 2, 1 },
            { 1, 2 },
            { 2, 2 }
        } 
    }, 
    // S PIECE:
    {
        // ROTATION 0:
        {
            { 1, 0 },
            { 2, 0 },
            { 0, 1 },
            { 1, 1 }
        },
        // ROTATION 1:
        {
            { 1, 0 },
            { 1, 1 },
            { 2, 1 },
            { 2, 2 },
        },
        // ROTATION 2:
        {
            { 1, 1 },
            { 2, 1 },
            { 0, 2 },
            { 1, 2 }
        },
        // ROTATION 3:
        {
            { 0, 0 },
            { 0, 1 },
            { 1, 1 },
            { 1, 2 }
        }
    }, 
    // Z PIECE:
    {
        // ROTATION 0:
        {
            { 0, 0 },
            { 1, 0 },
            { 1, 1 },
            { 2, 1 }
        },
        // ROTATION 1:
        {
            { 2, 0 },
            { 1, 1 },
            { 2, 1 },
            { 1, 2 }
        },
        // ROTATION 2:
        {
            { 0, 1 },
            { 1, 1 },
            { 1, 2 },
            { 2, 2 }
        },
        // ROTATION 3:
        {
            { 1, 0 },
            { 0, 1 },
            { 1, 1 },
            { 0, 2 }
        }
    }, 
    // L PIECE:
    {
        // ROTATION 0:
        {
            { 2, 0 },
            { 0, 1 },
            { 1, 1 },
            { 2, 1 }
        },
        // ROTATION 1:
        {
            { 1, 0 },
            { 1, 1 },
            { 1, 2 },
            { 2, 2 }
        },
        // ROTATION 2:
        {
            { 0, 1 },
            { 1, 1 },
            { 2, 1 },
            { 0, 2 }
        },
        // ROTATION 3:
        {
            { 0, 0 },
            { 1, 0 },
            { 1, 1 },
            { 1, 2 }
        }
    }, 
    // J PIECE:
    {
        
        // ROTATION 0:
        {
            { 0, 0 },
            { 0, 1 },
            { 1, 1 },
            { 2, 1 }
        },
        // ROTATION 1:
        {
            { 1, 0 },
            { 2, 0 },
            { 1, 1 },
            { 1, 2 }
        },
        // ROTATION 2:
        {
            { 0, 1 },
            { 1, 1 },
            { 2, 1 },
            { 2, 2 }
        },
        // ROTATION 2:
        {
            { 1, 0 },
            { 1, 1 },
            { 0, 2 },
            { 1, 2 }
        }
    }, 
    // T PIECE:
    {
        
        // ROTATION 0:
        {
            { 1, 0 },
            { 0, 1 },
            { 1, 1 },
            { 2, 1 }
        },
        // ROTATION 1:
        {
            { 1, 0 },
            { 1, 1 },
            { 2, 1 },
            { 1, 2 }
        },
        // ROTATION 2:
        {
            { 0, 1 },
            { 1, 1 },
            { 2, 1 },
            { 1, 2 }
        },
        // ROTATION 3:
        {
            { 1, 0 },
            { 0, 1 },
            { 1, 1 },
            { 1, 2 }
        }
    } 
};

static const int8_t piece_data_collision_checks[7][4][2][4][2] = 
{
    // I PIECE:
    {
        // FROM 0:
        {
            // LEFT:
            {
                { 0, -1 },
                { 0, 2 },
                { -2, -1 },
                { 1, 2 }
            },
            // RIGHT:
            {
                { 0, -2 },
                { 0, 1 },
                { 1, -2 },
                { -2, 1 }
            }
        },
        // FROM 1:
        {
            // LEFT:
            {
                { 0, 2 },
                { 0, -1 },
                { -1, 2 },
                { 2, -1 }
            },
            // RIGHT:
            {
                { 0, -1 },
                { 0, 2 },
                { -2, -1 },
                { 1, 2 }
            }
        },
        // FROM 2:
        {
            // LEFT:
            {
                { 0, 1 },
                { 0, -2 },
                { 2, 1 },
                { -1, -2 }
            },
            // RIGHT:
            {
                { 0, 2 },
                { 0, -1 },
                { -1, 2 },
                { 2, -1 }
            }
        },
        // FROM 3:
        {
            // LEFT:
            {
                { 0, -2 },
                { 0, 1 },
                { 1, -2 },
                { -2, 1 }
            },
            // RIGHT: 
            {
                { 0, 1 },
                { 0, -2 },
                { 2, 1 },
                { -1, -2 }
            }
        }
    },
    // O PIECE:
    {
        // FROM 0:
        {
            // LEFT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }, 
            // RIGHT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            }
        }, 
        // FROM 1:
        {
            // LEFT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            }
        },
        // FROM 2:
        {
            // LEFT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }
        },
        // FROM 3:
        {
            // LEFT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            },
            // RIGHT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            }
        }
    },
    // S PIECE:
    {
        // FROM 0:
        {
            // LEFT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }, 
            // RIGHT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            }
        }, 
        // FROM 1:
        {
            // LEFT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            }
        },
        // FROM 2:
        {
            // LEFT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }
        },
        // FROM 3:
        {
            // LEFT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            },
            // RIGHT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            }
        }
    },
    // Z PIECE:
    {
        // FROM 0:
        {
            // LEFT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }, 
            // RIGHT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            }
        }, 
        // FROM 1:
        {
            // LEFT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            }
        },
        // FROM 2:
        {
            // LEFT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }
        },
        // FROM 3:
        {
            // LEFT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            },
            // RIGHT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            }
        }
    }, 
    // L PIECE:
    {
        // FROM 0:
        {
            // LEFT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }, 
            // RIGHT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            }
        }, 
        // FROM 1:
        {
            // LEFT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            }
        },
        // FROM 2:
        {
            // LEFT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }
        },
        // FROM 3:
        {
            // LEFT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            },
            // RIGHT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            }
        }
    },
    // J PIECE:
    {
        // FROM 0:
        {
            // LEFT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }, 
            // RIGHT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            }
        }, 
        // FROM 1:
        {
            // LEFT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            }
        },
        // FROM 2:
        {
            // LEFT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }
        },
        // FROM 3:
        {
            // LEFT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            },
            // RIGHT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            }
        }
    },
    // T PIECE:
    {
        // FROM 0:
        {
            // LEFT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }, 
            // RIGHT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            }
        }, 
        // FROM 1:
        {
            // LEFT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { 1, 1 },
                { -2, 0 },
                { -2, 1 }
            }
        },
        // FROM 2:
        {
            // LEFT:
            {
                { 0, -1 },
                { -1, -1 },
                { 2, 0 },
                { 2, -1 }
            },
            // RIGHT:
            {
                { 0, 1 },
                { -1, 1 },
                { 2, 0 },
                { 2, 1 }
            }
        },
        // FROM 3:
        {
            // LEFT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            },
            // RIGHT:
            {
                { 0, -1 },
                { 1, -1 },
                { -2, 0 },
                { -2, -1 }
            }
        }
    } 
};
