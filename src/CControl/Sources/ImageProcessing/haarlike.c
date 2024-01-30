/*
 * haarlike_pattern.c
 *
 *  Created on: 27 Januari 2024
 *      Author: Daniel Mårtensson
 */

#include "imageprocessing.h"

/* Private functions */
INLINE static uint32_t compute_area(const uint32_t X[], const uint8_t column, const uint8_t x1, const uint8_t x2, const uint8_t y1, const uint8_t y2);

/*
 * Return the randomly genereated Haar-Like features 
 * This is made for Integral Image
 */
HAARLIKE_FEATURE* haarlike_pattern(const size_t total_haar_likes, const uint8_t row, const uint8_t column) {
	/* This is only for resetting the srand seed */
	randn(NULL, 0, 0.0f, 0.0f);

	/* Declare features */
	HAARLIKE_FEATURE* features = (HAARLIKE_FEATURE*)malloc(total_haar_likes * sizeof(HAARLIKE_FEATURE));
	memset(features, 0, total_haar_likes * sizeof(HAARLIKE_FEATURE));

	/* Create haar-like features */
	size_t i;
	for (i = 0; i < total_haar_likes; i++) {
		/* Do a random number from 0 to 5 */
		switch (randi(6)) {
		case 0:
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
			features[i].haarlike_feature_choice = HARLIIKE_FEATURE_CHOICE_EDGE_VERTICAL;
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
		case 1:
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
			features[i].haarlike_feature_choice = HARLIIKE_FEATURE_CHOICE_EDGE_HORIZONTAL;
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
		case 2:
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
			features[i].haarlike_feature_choice = HARLIIKE_FEATURE_CHOICE_LINE_VERTICAL;
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
		case 3:
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
			features[i].haarlike_feature_choice = HARLIIKE_FEATURE_CHOICE_LINE_VERTICAL;
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
		case 4:
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
			features[i].haarlike_feature_choice = HARLIIKE_FEATURE_CHOICE_CENTER;
			features[i].x1 = randi(column);
			features[i].y1 = randi(row);
			features[i].x4 = randi(column);
			features[i].y4 = randi(row);
			/* If x1 is larger than x4 */
			if (features[i].x1 > features[i].x4) {
				SWAP(features[i].x1, features[i].x4, uint8_t);
			}
			/* If y1 is larger than y4 */
			if (features[i].y1 > features[i].y4) {
				SWAP(features[i].y1, features[i].y4, uint8_t);
			}
			features[i].x2 = randi(features[i].x4 - features[i].x1 + 1) + features[i].x1;
			features[i].x3 = randi(features[i].x4 - features[i].x2 + 1) + features[i].x2;
			features[i].y2 = randi(features[i].y4 - features[i].y1 + 1) + features[i].y1;
			features[i].y3 = randi(features[i].y4 - features[i].y2 + 1) + features[i].y2;
			break;
		case 5:
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
			features[i].haarlike_feature_choice = HARLIIKE_FEATURE_CHOICE_SQUARES;
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
		}
	}

	/* Return features */
	return features;
}

int8_t haarlike_value(const uint32_t X[], const HAARLIKE_FEATURE* feature, const uint8_t row, const uint8_t column) {
	const HAARLIKE_FEATURE* f = feature;
	uint32_t white;
	uint32_t black;
	switch (f->haarlike_feature_choice) {
	case HARLIIKE_FEATURE_CHOICE_EDGE_VERTICAL:
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
	case HARLIIKE_FEATURE_CHOICE_EDGE_HORIZONTAL:
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
	case HARLIIKE_FEATURE_CHOICE_LINE_VERTICAL:
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
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y2) + compute_area(X, column, f->x3, f->x4, f->y1, f->y2);
		return white > black ? 1 : -1;
	case HARLIIKE_FEATURE_CHOICE_LINE_HORIZONTAL:
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
		black = compute_area(X, column, f->x1, f->x2, f->y1, f->y2) + compute_area(X, column, f->x1, f->x2, f->y3, f->y4);
		return white > black ? 1 : -1;
	case HARLIIKE_FEATURE_CHOICE_CENTER:
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
	case HARLIIKE_FEATURE_CHOICE_SQUARES:
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
		white = compute_area(X, column, f->x2, f->x3, f->y2, f->y3) + compute_area(X, column, f->x1, f->x2, f->y1, f->y2);
		black = compute_area(X, column, f->x1, f->x2, f->y2, f->y3) + compute_area(X, column, f->x2, f->x3, f->y1, f->y2);
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
