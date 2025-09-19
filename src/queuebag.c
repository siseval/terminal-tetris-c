#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <queuebag.h>
#include <piece.h>


struct queuebag* queuebag_create()
{
    struct queuebag* queuebag = malloc(sizeof(struct queuebag));

    queuebag->held_piece_type = NONE_TYPE;
    queuebag->can_hold = true;

    queuebag->bag_index = 0;
    for (uint8_t i = 0; i < PIECE_NUM_TYPES; i++)
    {
        queuebag->bag[i] = i;
    }
    queuebag_bag_shuffle(queuebag);

    memset(queuebag->queue, NONE_TYPE, sizeof(queuebag->queue));
    queuebag_queue_fill(queuebag);
    return queuebag;
}


void queuebag_bag_shuffle(struct queuebag* queuebag)
{
    for (uint8_t i = 0; i < PIECE_NUM_TYPES * 2; i++)
    {
        uint8_t first_index = rand() % PIECE_NUM_TYPES;
        uint8_t second_index = rand() % PIECE_NUM_TYPES;

        enum piece_type buffer = queuebag->bag[first_index];
        queuebag->bag[first_index] = queuebag->bag[second_index];
        queuebag->bag[second_index] = buffer;
    }
}

enum piece_type queuebag_bag_pull(struct queuebag* queuebag)
{
    enum piece_type pulled_type = queuebag->bag[queuebag->bag_index];
    queuebag->bag_index += 1;
    if (queuebag->bag_index >= PIECE_NUM_TYPES)
    {
        queuebag_bag_shuffle(queuebag);
        queuebag->bag_index = 0;
    }
    return pulled_type;
}

void queuebag_queue_fill(struct queuebag* queuebag)
{
    for (uint8_t i = 0; i < QUEUE_LENGTH; i++)
    {
        queuebag->queue[i] = queuebag->bag[queuebag->bag_index];
        queuebag->bag_index += 1;
        if (queuebag->bag_index >= PIECE_NUM_TYPES)
        {
            queuebag_bag_shuffle(queuebag);
        }
    }
}

enum piece_type queuebag_queue_pull(struct queuebag* queuebag)
{
    enum piece_type pulled_type = queuebag->queue[0];
    for (uint8_t i = 0; i < QUEUE_LENGTH - 1; i++)
    {
        queuebag->queue[i] = queuebag->queue[i + 1];
    }
    queuebag->queue[QUEUE_LENGTH - 1] = queuebag_bag_pull(queuebag);
    return pulled_type;
}
