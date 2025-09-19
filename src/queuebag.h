#ifndef QUEUE_H
#define QUEUE_H

#include <piece.h>

#define QUEUE_LENGTH 5

struct queuebag
{
    enum piece_type held_piece_type;
    bool can_hold;
    enum piece_type queue[QUEUE_LENGTH];
    enum piece_type bag[PIECE_NUM_TYPES];
    uint8_t bag_index;
};

struct queuebag* queuebag_create();

void queuebag_bag_shuffle(struct queuebag* queuebag);
enum piece_type queuebag_bag_pull(struct queuebag* queuebag);

enum piece_type queuebag_queue_pull(struct queuebag* queuebag);
void queuebag_queue_fill(struct queuebag* queuebag);

void queuebag_hold_piece(struct queuebag* queuebag);

#endif
