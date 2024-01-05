/*
 * anglevectors.c
 *
 *  Created on: Nov 18, 2023
 *      Author: Daniel Mårtensson
 */

#include "miscellaneous.h"

/* 
 * Compute the angle between two vectors
 */
float anglevector(const float a[], const float b[], const size_t row) {
    /* Calculate the dot product */
    float dotProduct = dot(a, b, row);

    /* Calculate the lengths of the vectors */
    float lengthA = norm(a, row, 1, NORM_METHOD_L2);
    float lengthB = norm(b, row, 1, NORM_METHOD_L2);

    /* Calculate the cosine of the angle */
    float cosineAngle = dotProduct / (lengthA * lengthB);

    /* Ensure that cosineAngle is within the range[-1, 1] */
    cosineAngle = vmin(vmax(cosineAngle, -1.0f), 1.0f);

    /* Use atan2 to get the angle in radians */
    float angleRadians = atan2f(sqrtf(1.0f - cosineAngle * cosineAngle), cosineAngle);

    /* Convert the angle from radians to degrees */
    float angleDegrees = rad2deg(angleRadians);

    /* Return */
    return angleDegrees;
}

/* GNU octave code
function angleDegrees = anglevector(a, b)
  % Calculate the dot product
  dotProduct = dot(a, b);

  % Calculate the lengths of the vectors
  lengthA = norm(a, 2);
  lengthB = norm(b, 2);

  % Calculate the cosine of the angle
  cosineAngle = dotProduct / (lengthA * lengthB);

  % Ensure that cosineAngle is within the range[-1, 1]
  cosineAngle = min(max(cosineAngle, -1), 1);

  % Use atan2 to get the angle in radians
  angleRadians = atan2(sqrt(1 - cosineAngle * cosineAngle), cosineAngle);

  % Convert the angle from radians to degrees
  angleDegrees = rad2deg(angleRadians);
end
*/