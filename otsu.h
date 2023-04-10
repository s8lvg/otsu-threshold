#include <stdint.h>
#include <stdlib.h>

int comp(const void * elem1,
  const void * elem2) {
  int f = * ((int * ) elem1);
  int s = * ((int * ) elem2);
  if (f > s) return 1;
  if (f < s) return -1;
  return 0;
}

static uint64_t findMedianSorted(uint64_t * v, int offset, int len) {
  if (len % 2 == 0) {
    return (v[(len + offset) / 2 - 1] + v[(len + offset) / 2]) / 2;
  } else {
    return v[(len + offset) / 2];
  }
}

static uint64_t otsu_threshold(uint64_t * measures, int len) {
  // Sort array
  qsort(measures, len, sizeof(size_t), comp);
  
#ifdef FILTER_OUTLIER
  // Prefilter to remove outlier
  uint64_t Q1 = findMedianSorted(measures, 0, len);
  uint64_t Q3 = findMedianSorted(measures, len / 2, len);
  uint64_t IQR = Q3 - Q1;
  int count = 0;
  for (size_t i = 0; i < len; i++) {
    if (measures[i] < Q1 - 1.5 * IQR || measures[i] > Q3 + 1.5 * IQR) {
      measures[count] = measures[i];
      count++;
    }
  }
  len = count;
#endif

  // Normalize array entries
  uint64_t min = measures[0];
  uint64_t max = measures[len - 1];
  for (size_t i = 0; i < len; i++) {
    measures[i] = ((measures[i] - min) * 255) / ((float)(max - min));
  }

  // Build a histogram for the array
  uint64_t hist[256] = {0};
  for (size_t i = 0; i < len; i++) {
    hist[measures[i]]++;
  }

  // Compute threshold
  float sum = 0;
  float sumB = 0;
  int q1 = 0;
  int q2 = 0;
  float varMax = 0;
  int threshold = 0;

  // Auxiliary value for computing m2
  for (int i = 0; i <= 255; i++) {
    sum += i * ((int) hist[i]);
  }

  for (int i = 0; i <= 255; i++) {
    // Update q1
    q1 += hist[i];
    if (q1 == 0) continue;

    // Update q2
    q2 = len - q1;
    if (q2 == 0) break;

    // Update m1 and m2
    sumB += (float)(i * ((int) hist[i]));
    float m1 = sumB / q1;
    float m2 = (sum - sumB) / q2;

    // Update the between class variance
    float varBetween = (float) q1 * (float) q2 * (m1 - m2) * (m1 - m2);

    // Update the threshold if necessary
    if (varBetween > varMax) {
      varMax = varBetween;
      threshold = i;
    }
  }

  // Denormalize treshold to get usable value
  uint64_t denormalized_threshold = ((threshold * (max - min)) / 255) + min;
  return denormalized_threshold;
}