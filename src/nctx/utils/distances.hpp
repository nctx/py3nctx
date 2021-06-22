#ifndef _UTL_DST_H
#define _UTL_DST_H


#include <cmath>
#include <algorithm>
#include <numeric> 
#include <functional> 

namespace nctx{


  // DocString: kl_divergence
  /**
   * @brief Obtain Kullback Leibler Divergence
   * 
   * A straightforward implementation of KL Divergence. Note that both lists must be probability distributions, i.e. they sum to one. This is not checked internally and will result in obscure results if not taken care of.
   * 
   * The KL Divergence does not fulfil triangle inequality and, hence, is no metric. See https://en.wikipedia.org/wiki/Kullback%E2%80%93Leibler_divergence for more information.
   * Both lists feeded into this function must have the same length and contain only numeric values.
   * 
   */
  template <typename DIter>
  inline double_t kl_divergence(DIter begin1, DIter end1, DIter begin2, DIter end2) {
    //~ using T = typename DIter::value_type;
    std::vector<double_t>	auxiliary;
    std::transform (begin1, end1, begin2, std::back_inserter(auxiliary), [](double_t element1, double_t element2) {
      if(element1 > 0 && element2 > 0)
        return std::log(element1 / element2) * element1;
      else return 0.0;
    });
    //~ return std::reduce(std::execution::par, auxiliary.begin(), auxiliary.end());
    return std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0);
  }
  
  // DocString: js_divergence
  /**
   * @brief Obtain Jenson Shannon Divergence
   * 
   * A straightforward implementation of JS Divergence. Note that both lists must be probability distributions, i.e. they sum to one. This is not checked internally and will result in obscure results if not taken care of.
   * 
   * The JS Divergence is based on the KL Divergence. Its square root yields a metric. See https://en.wikipedia.org/wiki/Jensen%E2%80%93Shannon_divergence for more information.
   * Both lists feeded into this function must have the same length and contain only numeric values.
   * 
   */
  template <typename DIter>
  inline double_t js_divergence(DIter begin1, DIter end1, DIter begin2, DIter end2) {
    //~ using T = typename DIter::value_type;
    // copies needed here in case we are passed a vector of ints...
    std::vector<double_t> one(begin1, end1);
    std::vector<double_t> two(begin2, end2);
      
    std::vector<double_t>	auxiliary; 
    
    std::transform (begin(one), end(one), begin(two), std::back_inserter(auxiliary), [](double_t element1, double_t element2) {
      return element1 + element2;
    }); // one+two
    std::transform(begin(auxiliary),end(auxiliary), begin(auxiliary),
               std::bind(std::multiplies<double_t>(), std::placeholders::_1, 0.5)); // 1/2 * auxiliary
    return (0.5 * kl_divergence(begin(one), end(one), begin(auxiliary), end(auxiliary))) + 
           (0.5 * kl_divergence(begin(two), end(two), begin(auxiliary), end(auxiliary)));
  }
  
  
  // DocString: eucl_distance
  /**
   * @brief Obtain Euclidean Distance
   * 
   * A straightforward implementation of Euclidean Distance.
   * 
   * See https://en.wikipedia.org/wiki/Euclidean_distance for more information.
   * Both lists feeded into this function must have the same length and contain only numeric values.
   * 
   */
  template <typename DIter>
  inline double_t euclidean_distance(DIter begin1, DIter end1, DIter begin2, DIter end2) {
    std::vector<double_t>	auxiliary;
    //~ using T = typename DIter::value_type;
    std::transform (begin1, end1, begin2, std::back_inserter(auxiliary), [](double_t element1, double_t element2) {
      return pow((element1-element2),2.0);
    });
    return sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
  }
  
  
  // DocString: cosine_sim
  /**
   * @brief Obtain Cosine Similarity
   * 
   * A straightforward implementation of Cosine Similarity.
   * 
   * See https://en.wikipedia.org/wiki/Cosine_similarity for more information.
   * Both lists feeded into this function must have the same length and contain only numeric values.
   * 
   */
  template <typename DIter>
  inline double_t cosine_similarity(DIter begin1, DIter end1, DIter begin2, DIter end2) {
    double_t prod = std::inner_product(begin1, end1, begin2, 0.0);
    double_t one = std::inner_product(begin1, end1, begin1, 0.0);
    double_t two = std::inner_product(begin2, end2, begin2, 0.0);
    return prod / sqrt(one*two);
  }
  
  
  // DocString: angular_distance
  /**
   * @brief Obtain Angular Distance
   * 
   * A straightforward implementation of Angular Distance.
   * 
   * The Angular Distance is a metric based on the KL Divergence. See https://en.wikipedia.org/wiki/Cosine_similarity#Angular_distance_and_similarity for more information.
   * Both lists feeded into this function must have the same length and contain only numeric values.
   * 
   */
  template <typename DIter>
  inline double_t angular_distance(DIter begin1, DIter end1, DIter begin2, DIter end2) {
    double_t prod = std::inner_product(begin1, end1, begin2, 0.0);
    double_t one = std::inner_product(begin1, end1, begin1, 0.0);
    double_t two = std::inner_product(begin2, end2, begin2, 0.0);
    double_t _cos = prod / sqrt(one*two);
    return std::acos(_cos)/M_PI;
  }
  
}//nproc

#endif
