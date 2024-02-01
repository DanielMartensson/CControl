/*
 * haarlike.c
 *
 *  Created on: 27 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/* Private functions */
INLINE static uint32_t compute_area(const uint32_t X[], const uint8_t column, const uint8_t x1, const uint8_t x2, const uint8_t y1, const uint8_t y2);
INLINE static void generate_9x9(HAARLIKE_FEATURE* feature, const HAARLIKE_FEATURE_CHOICE feature_choice, const uint8_t row, const uint8_t column);

/*
 * Return the randomly genereated Haar-Like features 
 * This is made for Integral Image
 */
HAARLIKE_FEATURE* haarlike_features(const size_t total_haar_likes, const uint8_t row, const uint8_t column) {
	/* This is only for resetting the srand seed */
	randn(NULL, 0, 0.0f, 0.0f);

	/* Generate features */
	HAARLIKE_FEATURE* features = (HAARLIKE_FEATURE*)malloc(total_haar_likes * sizeof(HAARLIKE_FEATURE));
	memset(features, 0, total_haar_likes * sizeof(HAARLIKE_FEATURE));

	/* Create haar-like features */
	size_t i;
	for (i = 0; i < total_haar_likes; i++) {
		/* Do a random number from 0 to HAARLIKE_FEATURE_TOTAL_HAARLIKES - 1*/
		const HAARLIKE_FEATURE_CHOICE feature_choice = randi(HAARLIKE_FEATURE_TOTAL_HAARLIKES);
		switch (feature_choice) {
		case HAARLIKE_FEATURE_CHOICE_EDGE_VERTICAL:
			/* Create two boxes - Black and white
			 * (x1, y1)  (x2, y1)   (x3, y1)
			 * o__________o__________o 
			 * |          |          |
			 * |          |          |
			 * |          |          |
			 * |          |          |
			 * |  Black   |   White  |
			 * |          |          |
			 * |          |          |
			 * |          |          |
			 * o__________o__________o 
			 * (x1, y2)  (x2, y2)   (x3, y2)
			 */
			features[i].haarlike_feature_choice = HAARLIKE_FEATURE_CHOICE_EDGE_VERTICAL;
			features[i].x1 = randi(column);
			features[i].y1 = randi(row);
			features[i].x3 = randi(column);
			features[i].y2 = randi(row);
			/* If x1 is larger than x3 */
			if (features[i].x1 > features[i].x3) {
				SWAP(features[i].x1, features[i].x3, uint8_t);
			}
			/* If y1 is larger than y2 */
			if (features[i].y1 > features[i].y2) {
				SWAP(features[i].y1, features[i].y2, uint8_t);
			}
			features[i].x2 = randi(features[i].x3 - features[i].x1 + 1) + features[i].x1;
			break;
		case HAARLIKE_FEATURE_CHOICE_EDGE_HORIZONTAL:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____________________o (x2, y1)
			 * |                     |
			 * |          Black      |
			 * | (x1, y2)            |
			 * o_____________________o (x2, y2)
			 * |                     |
			 * |                     |
			 * |         White       |
			 * |                     |
			 * o_____________________o (x2, y3)
			 *   (x1, y3)
			 */
			features[i].haarlike_feature_choice = HAARLIKE_FEATURE_CHOICE_EDGE_HORIZONTAL;
			features[i].x1 = randi(column);
			features[i].y1 = randi(row);
			features[i].x2 = randi(column);
			features[i].y3 = randi(row);
			/* If x1 is larger than x2 */
			if (features[i].x1 > features[i].x2) {
				SWAP(features[i].x1, features[i].x2, uint8_t);
			}
			/* If y1 is larger than y3 */
			if (features[i].y1 > features[i].y3) {
				SWAP(features[i].y1, features[i].y3, uint8_t);
			}
			features[i].y2 = randi(features[i].y3 - features[i].y1 + 1) + features[i].y1;
			break;
		case HAARLIKE_FEATURE_CHOICE_LINE_VERTICAL:
			/* Create three boxes - Black and white
			 * (x1, y1)
			 * o______o________o________o (x4, y1)
			 * |      |(x2, y1)|(x3, y1)|
			 * |      |        |        |
			 * |      |        |        |
			 * | Black|  White | Black  |
			 * |      |        |        |
			 * |      |        |        |
			 * |      |        |        |
			 * |      |        |        |
			 * o______o________o________o (x4, y2)
			 * (x1, y2) (x2, y2) (x3, y2)|
			 */
			features[i].haarlike_feature_choice = HAARLIKE_FEATURE_CHOICE_LINE_VERTICAL;
			features[i].x1 = randi(column);
			features[i].y1 = randi(row);
			features[i].x4 = randi(column);
			features[i].y2 = randi(row);
			/* If x1 is larger than x4 */
			if (features[i].x1 > features[i].x4) {
				SWAP(features[i].x1, features[i].x4, uint8_t);
			}
			/* If y1 is larger than y2 */
			if (features[i].y1 > features[i].y2) {
				SWAP(features[i].y1, features[i].y2, uint8_t);
			}
			features[i].x2 = randi(features[i].x4 - features[i].x1 + 1) + features[i].x1;
			features[i].x3 = randi(features[i].x4 - features[i].x2 + 1) + features[i].x2;
			break;
		case HAARLIKE_FEATURE_CHOICE_LINE_HORIZONTAL:
			/* Create three boxes - Black and white
			 * (x1, y1)
			 * o________________________o (x2, y1)
			 * |                        |
			 * |         Black          |
			 * o------------------------o (x2, y2)
			 * |(x1, y2) White          |
			 * |                        |
			 * o------------------------o (x2, y3)
			 * |(x1, y3) Black          |
			 * |                        |
			 * o________________________o (x2, y4)
			 * (x1, y4)
			 */
			features[i].haarlike_feature_choice = HAARLIKE_FEATURE_CHOICE_LINE_HORIZONTAL;
			features[i].x1 = randi(column);
			features[i].y1 = randi(row);
			features[i].x2 = randi(column);
			features[i].y4 = randi(row);
			/* If x1 is larger than x2 */
			if (features[i].x1 > features[i].x2) {
				SWAP(features[i].x1, features[i].x2, uint8_t);
			}
			/* If y1 is larger than y4 */
			if (features[i].y1 > features[i].y4) {
				SWAP(features[i].y1, features[i].y4, uint8_t);
			}
			features[i].y2 = randi(features[i].y4 - features[i].y1 + 1) + features[i].y1;
			features[i].y3 = randi(features[i].y4 - features[i].y2 + 1) + features[i].y2;
			break;
		case HAARLIKE_FEATURE_CHOICE_CENTER:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |    (x2, y1) (x3, y1)|
			 * |    (x2, y2) (x3, y2)|
			 * |     o_________o     |
			 * |     |         |     |
			 * |     |  Black  |     |
			 * |     o_________o     |
			 * |  (x2, y3)  (x3, y3) |
			 * |        White        |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_SQUARES:
			/* Create four boxes - Black and white
			 * (x1, y1)  (x2, y1)    (x3, y1)
			 * o__________o__________o
			 * |          |          |
			 * |  White   | Black    |
			 * |          |          |
			 * |          |          |
			 * o----------o----------o (x3, y2)
			 * |(x1, y2)  |(x2, y2)  |
			 * |  Black   |  White   |
			 * |          |          |
			 * o__________o__________o (x3, y3)
			 * (x1, y3)   (x2, y3)
			 */
			features[i].haarlike_feature_choice = HAARLIKE_FEATURE_CHOICE_SQUARES;
			features[i].x1 = randi(column);
			features[i].y1 = randi(row);
			features[i].x3 = randi(column);
			features[i].y3 = randi(row);
			/* If x1 is larger than x3 */
			if (features[i].x1 > features[i].x3) {
				SWAP(features[i].x1, features[i].x3, uint8_t);
			}
			/* If y1 is larger than y3 */
			if (features[i].y1 > features[i].y3) {
				SWAP(features[i].y1, features[i].y3, uint8_t);
			}
			features[i].x2 = randi(features[i].x3 - features[i].x1 + 1) + features[i].x1;
			features[i].y2 = randi(features[i].y3 - features[i].y1 + 1) + features[i].y1;
			break;
		case HAARLIKE_FEATURE_CHOICE_CROSS:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  Black  | Bl  |         
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  Black  | Bl  |         
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_UP_U:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  White  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_DOWN_U:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  White  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_X:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_UP_T:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  White  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_DOWN_T:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  White  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_TILTED_LEFT_T:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Wh  |  White  | Wh  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_TILTED_RIGHT_T:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Bl  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Wh  |  White  | Wh  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Bl  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_UP_L:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Bl  |  White  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  White  | Wh  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Bl  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_DOWN_L:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Bl  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  White  | Wh  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Bl  |  White  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_UP_MIRROR_L:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  White  | Bl  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Wh  |  White  | Bl  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Bl  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_DOWN_MIRROR_L:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Bl  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Wh  |  White  | Bl  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  White  | Bl  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_H:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Bl  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Bl  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_TILTED_H:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  Black  | Bl  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_UP_Y:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_DOWN_Y:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  White  | Bl  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_TILTED_LEFT_Y:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Wh  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Bl  |  White  | Wh  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Wh  |  Black  | Bl  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		case HAARLIKE_FEATURE_CHOICE_TILTED_RIGHT_Y:
			/* Create two boxes - Black and white
			 * (x1, y1)
			 * o_____o_________o_____o (x4, y1)
			 * |     |         |     |
			 * | Bl  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y2)
			 * |     |         |     |
			 * | Wh  |  White  | Bl  |
			 * o-----o---------o-----o (x4, y3)
			 * |     |         |     |
			 * | Bl  |  Black  | Wh  |
			 * o_____o_________o_____o (x4, y4)
			 * (x1, y4) (x2, y4) (x3, y4)
			 */
			generate_9x9(&features[i], feature_choice, row, column);
			break;
		}
	}

	/* Return features */
	return features;
}

int8_t haarlike_predict(const uint32_t X[], const HAARLIKE_FEATURE* feature, const uint8_t row, const uint8_t column) {
	const HAARLIKE_FEATURE* f = feature;
	uint32_t white;
	uint32_t black;
	switch (f->haarlike_feature_choice) {
	case HAARLIKE_FEATURE_CHOICE_EDGE_VERTICAL:
		/* 
		 * (x1, y1)  (x2, y1)   (x3, y1)
		 * o__________o__________o
		 * |          |          |
		 * |          |          |
		 * |          |          |
		 * |          |          |
		 * |  Black   |   White  |
		 * |          |          |
		 * |          |          |
		 * |          |          |
		 * o__________o__________o
		 * (x1, y2)  (x2, y2)   (x3, y2)
		 */
		white = compute_area(X, column, f->x2, f->x3, f->y1, f->y2);
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y2);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_EDGE_HORIZONTAL:
		/* 
		 * (x1, y1)
		 * o_____________________o (x2, y1)
		 * |                     |
		 * |          Black      |
		 * | (x1, y2)            |
		 * o_____________________o (x2, y2)
		 * |                     |
		 * |                     |
		 * |         White       |
		 * |                     |
		 * o_____________________o (x2, y3)
		 *   (x1, y3)
		 */
		white = compute_area(X, column, f->x1, f->x2, f->y2, f->y3);
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y2);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_LINE_VERTICAL:
		/* 
		 * (x1, y1)
		 * o______o________o________o (x4, y1)
		 * |      |(x2, y1)|(x3, y1)|
		 * |      |        |        |
		 * |      |        |        |
		 * | Black|  White | Black  |
		 * |      |        |        |
		 * |      |        |        |
		 * |      |        |        |
		 * |      |        |        |
		 * o______o________o________o (x4, y2)
		 * (x1, y2) (x2, y2) (x3, y2)|
		 */
		white = compute_area(X, column, f->x2, f->x3, f->y1, f->y2);
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y2) + 
				compute_area(X, column, f->x3, f->x4, f->y1, f->y2);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_LINE_HORIZONTAL:
		/* 
		 * (x1, y1)
		 * o________________________o (x2, y1)
		 * |                        |
		 * |         Black          |
		 * o------------------------o (x2, y2)
		 * |(x1, y2) White          |
		 * |                        |
		 * o------------------------o (x2, y3)
		 * |(x1, y3) Black          |
		 * |                        |
		 * o________________________o (x2, y4)
		 * (x1, y4)
		 */
		white = compute_area(X, column, f->x1, f->x2, f->y2, f->y3);
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y2) + 
				compute_area(X, column, f->x1, f->x2, f->y3, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_CENTER:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |    (x2, y1) (x3, y1)|
		 * |    (x2, y2) (x3, y2)|
		 * |     o_________o     |
		 * |     |         |     |
		 * |     |  Black  |     |
		 * |     o_________o     |
		 * |  (x2, y3)  (x3, y3) |
		 * |        White        |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x3, f->x4, f->y1, f->y4) +
				compute_area(X, column, f->x1, f->x2, f->y1, f->y4) +
				compute_area(X, column, f->x2, f->x3, f->y3, f->y4) +
				compute_area(X, column, f->x2, f->x3, f->y1, f->y2);
		black = compute_area(X, column, f->x2, f->x3, f->y2, f->y3);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_SQUARES:
		/* 
		 * (x1, y1)  (x2, y1)    (x3, y1)
		 * o__________o__________o
		 * |          |          |
		 * |  White   | Black    |
		 * |          |          |
		 * |          |          |
		 * o----------o----------o (x3, y2)
		 * |(x1, y2)  |(x2, y2)  |
		 * |  Black   |  White   |
		 * |          |          |
		 * o__________o__________o (x3, y3)
		 * (x1, y3)   (x2, y3)
		 */
		white = compute_area(X, column, f->x2, f->x3, f->y2, f->y3) + 
				compute_area(X, column, f->x1, f->x2, f->y1, f->y2);
		black = compute_area(X, column, f->x1, f->x2, f->y2, f->y3) + 
				compute_area(X, column, f->x2, f->x3, f->y1, f->y2);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_CROSS:
		/* Create two boxes - Black and white
	     * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  Black  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x2, f->y1, f->y2) +
				compute_area(X, column, f->x3, f->x4, f->y1, f->y2) +
				compute_area(X, column, f->x1, f->x2, f->y3, f->y4) +
				compute_area(X, column, f->x3, f->x4, f->y3, f->y4);
		black = compute_area(X, column, f->x2, f->x3, f->y1, f->y2) + 
				compute_area(X, column, f->x1, f->x4, f->y2, f->y3) +
				compute_area(X, column, f->x2, f->x3, f->y3, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_UP_U:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Wh  |  White  | Wh  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x2, f->y1, f->y4) +
				compute_area(X, column, f->x2, f->x3, f->y3, f->y4) +
				compute_area(X, column, f->x3, f->x4, f->y1, f->y4);
		black = compute_area(X, column, f->x2, f->x3, f->y1, f->y3);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_DOWN_U:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Wh  |  White  | Wh  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x2, f->y1, f->y4) +
				compute_area(X, column, f->x2, f->x3, f->y1, f->y2) +
				compute_area(X, column, f->x3, f->x4, f->y1, f->y4);
		black = compute_area(X, column, f->x2, f->x3, f->y2, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_X:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x2, f->y1, f->y2) +
				compute_area(X, column, f->x3, f->x4, f->y1, f->y2) +
				compute_area(X, column, f->x2, f->x3, f->y2, f->y3) +
				compute_area(X, column, f->x1, f->x2, f->y3, f->y4) +
				compute_area(X, column, f->x3, f->x4, f->y3, f->y4);
		black = compute_area(X, column, f->x2, f->x3, f->y1, f->y2) +
				compute_area(X, column, f->x1, f->x2, f->y2, f->y3) +
				compute_area(X, column, f->x3, f->x4, f->y2, f->y3) +
				compute_area(X, column, f->x2, f->x3, f->y3, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_UP_T:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Wh  |  White  | Wh  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x4, f->y1, f->y2) +
				compute_area(X, column, f->x2, f->x3, f->y2, f->y4);
		black = compute_area(X, column, f->x1, f->x2, f->y2, f->y4) +
				compute_area(X, column, f->x3, f->x4, f->y2, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_DOWN_T:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Wh  |  White  | Wh  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x2, f->x3, f->y1, f->y3) +
				compute_area(X, column, f->x1, f->x4, f->y3, f->y4);
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y3) +
				compute_area(X, column, f->x3, f->x4, f->y1, f->y3);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_TILTED_LEFT_T:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Wh  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Wh  |  White  | Wh  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Wh  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x2, f->y1, f->y4) +
				compute_area(X, column, f->x2, f->x4, f->y2, f->y3);
		black = compute_area(X, column, f->x2, f->x4, f->y1, f->y2) +
				compute_area(X, column, f->x2, f->x4, f->y3, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_TILTED_RIGHT_T:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Bl  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Wh  |  White  | Wh  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Bl  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x3, f->x4, f->y1, f->y4) +
				compute_area(X, column, f->x1, f->x3, f->y2, f->y3);
		black = compute_area(X, column, f->x1, f->x3, f->y1, f->y2) +
				compute_area(X, column, f->x1, f->x3, f->y3, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_UP_L:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Bl  |  White  | Wh  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  White  | Wh  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Bl  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x2, f->x4, f->y1, f->y3);
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y4) +
				compute_area(X, column, f->x2, f->x4, f->y3, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_DOWN_L:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Bl  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  White  | Wh  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Bl  |  White  | Wh  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x2, f->x4, f->y2, f->y4);
		black = compute_area(X, column, f->x1, f->x4, f->y1, f->y2) +
				compute_area(X, column, f->x1, f->x2, f->y2, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_UP_MIRROR_L:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Wh  |  White  | Bl  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Wh  |  White  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Bl  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x3, f->y1, f->y3);
		black = compute_area(X, column, f->x1, f->x4, f->y3, f->y4) +
				compute_area(X, column, f->x3, f->x4, f->y1, f->y3);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_DOWN_MIRROR_L:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Bl  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Wh  |  White  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Wh  |  White  | Bl  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x3, f->y2, f->y4);
		black = compute_area(X, column, f->x1, f->x4, f->y1, f->y2) +
				compute_area(X, column, f->x3, f->x4, f->y2, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_H:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Bl  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Bl  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x2, f->y2, f->y3) +
				compute_area(X, column, f->x3, f->x4, f->y2, f->y3);
		black = compute_area(X, column, f->x1, f->x4, f->y1, f->y2) +
				compute_area(X, column, f->x1, f->x4, f->y3, f->y4) +
				compute_area(X, column, f->x2, f->x3, f->y2, f->y3);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_TILTED_H:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  Black  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x2, f->x3, f->y1, f->y2) +
				compute_area(X, column, f->x2, f->x3, f->y3, f->y4);
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y4) +
				compute_area(X, column, f->x3, f->x4, f->y1, f->y4) +
				compute_area(X, column, f->x2, f->x3, f->y2, f->y3);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_UP_Y:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x2, f->x3, f->y2, f->y4) +
				compute_area(X, column, f->x1, f->x2, f->y1, f->y2) +
				compute_area(X, column, f->x3, f->x4, f->y1, f->y2);
		black = compute_area(X, column, f->x1, f->x2, f->y2, f->y4) +
				compute_area(X, column, f->x3, f->x4, f->y2, f->y4) +
				compute_area(X, column, f->x2, f->x3, f->y1, f->y2);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_DOWN_Y:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  White  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Wh  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x2, f->x3, f->y1, f->y3) +
				compute_area(X, column, f->x1, f->x2, f->y3, f->y4) +
				compute_area(X, column, f->x3, f->x4, f->y3, f->y4);
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y3) +
				compute_area(X, column, f->x3, f->x4, f->y1, f->y3) +
				compute_area(X, column, f->x2, f->x3, f->y3, f->y4);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_TILTED_LEFT_Y:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Wh  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Bl  |  White  | Wh  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Wh  |  Black  | Bl  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x1, f->x2, f->y1, f->y2) +
				compute_area(X, column, f->x1, f->x2, f->y3, f->y4) +
				compute_area(X, column, f->x2, f->x4, f->y2, f->y3);
		black = compute_area(X, column, f->x2, f->x4, f->y1, f->y2) +
				compute_area(X, column, f->x2, f->x4, f->y3, f->y4) +
				compute_area(X, column, f->x1, f->x2, f->y2, f->y3);
		return white > black ? 1 : -1;
	case HAARLIKE_FEATURE_CHOICE_TILTED_RIGHT_Y:
		/* Create two boxes - Black and white
		 * (x1, y1)
		 * o_____o_________o_____o (x4, y1)
		 * |     |         |     |
		 * | Bl  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y2)
		 * |     |         |     |
		 * | Wh  |  White  | Bl  |
		 * o-----o---------o-----o (x4, y3)
		 * |     |         |     |
		 * | Bl  |  Black  | Wh  |
		 * o_____o_________o_____o (x4, y4)
		 * (x1, y4) (x2, y4) (x3, y4)
		 */
		white = compute_area(X, column, f->x3, f->x4, f->y1, f->y2) +
				compute_area(X, column, f->x1, f->x3, f->y2, f->y3) +
				compute_area(X, column, f->x3, f->x4, f->y3, f->y4);
		black = compute_area(X, column, f->x1, f->x3, f->y1, f->y2) +
				compute_area(X, column, f->x1, f->x3, f->y3, f->y4) +
				compute_area(X, column, f->x3, f->x4, f->y2, f->y3);
		return white > black ? 1 : -1;
	}
}

INLINE static uint32_t compute_area(const uint32_t X[], const uint8_t column, const uint8_t x1, const uint8_t x2, const uint8_t y1, const uint8_t y2) {
	const uint32_t A = X[y1 * column + x1];
	const uint32_t B = X[y1 * column + x2] - A;
	const uint32_t C = X[y2 * column + x1] - A;
	const uint32_t D = X[y2 * column + x2] - C - B - A;
	return D;
}

INLINE static void generate_9x9(HAARLIKE_FEATURE* feature, const HAARLIKE_FEATURE_CHOICE feature_choice, const uint8_t row, const uint8_t column) {
	feature->haarlike_feature_choice = feature_choice;
	feature->x1 = randi(column);
	feature->y1 = randi(row);
	feature->x4 = randi(column);
	feature->y4 = randi(row);
	/* If x1 is larger than x4 */
	if (feature->x1 > feature->x4) {
		SWAP(feature->x1, feature->x4, uint8_t);
	}
	/* If y1 is larger than y4 */
	if (feature->y1 > feature->y4) {
		SWAP(feature->y1, feature->y4, uint8_t);
	}
	feature->x2 = randi(feature->x4 - feature->x1 + 1) + feature->x1;
	feature->x3 = randi(feature->x4 - feature->x2 + 1) + feature->x2;
	feature->y2 = randi(feature->y4 - feature->y1 + 1) + feature->y1;
	feature->y3 = randi(feature->y4 - feature->y2 + 1) + feature->y2;
}