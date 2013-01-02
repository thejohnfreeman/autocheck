#ifndef AUTOCHECK_CLASSIFIER_HPP
#define AUTOCHECK_CLASSIFIER_HPP

#include <unordered_map>

#include "function.hpp"
#include "apply.hpp"

namespace autocheck {

  template <typename... Args>
  class classifier {
    public:
      typedef typename predicate<Args...>::type pred_t;
      typedef std::tuple<pred_t, std::string>   tagger_t;

    private:
      pred_t                                  is_trivial;
      size_t                                  num_trivial;
      std::vector<tagger_t>                   taggers;
      std::unordered_map<std::string, size_t> tags;

    public:
      classifier() :
        is_trivial(never()), num_trivial(0), taggers(), tags() {}

      classifier& trivial(const pred_t& pred) {
        is_trivial = pred;
        return *this;
      }

      classifier& classify(const pred_t& pred, const std::string& label) {
        taggers.push_back(tagger_t(pred, label));
        return *this;
      }

      void check(const std::tuple<Args...>& args) {
        if (apply(is_trivial, args)) ++num_trivial;

        std::string tag;
        for (tagger_t& tagger : taggers) {
          if (apply(std::get<0>(tagger), args)) {
            if (!tag.empty()) tag += ", ";
            tag += std::get<1>(tagger);
          }
        }
        if (!tag.empty()) ++tags[tag];
      }

      size_t trivial() const { return num_trivial; }

      distribution distro() const {
        return distribution(tags.begin(), tags.end());
      }
  };

  template <typename... Args>
  classifier<Args...>&& trivial(
      const typename classifier<Args...>::pred_t& pred,
      classifier<Args...>&& cls)
  {
    cls.trivial(pred);
    return std::forward<classifier<Args...>>(cls);
  }

  template <typename... Args>
  classifier<Args...>&& classify(
      const typename classifier<Args...>::pred_t& pred,
      const std::string& label,
      classifier<Args...>&& cls)
  {
    cls.classify(pred, label);
    return std::forward<classifier<Args...>>(cls);
  }

}

#endif

