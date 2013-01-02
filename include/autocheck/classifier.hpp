#ifndef AUTOCHECK_CLASSIFIER_HPP
#define AUTOCHECK_CLASSIFIER_HPP

#include <unordered_map>

#include "function.hpp"
#include "apply.hpp"

namespace autocheck {

  template <typename... Args>
  class classifier {
    public:
      typedef typename predicate<Args...>::type           pred_t;
      typedef std::function<std::string (const Args&...)> tagger_t;

    private:
      pred_t                                  is_trivial;
      size_t                                  num_trivial;
      std::vector<tagger_t>                   taggers;
      std::unordered_map<std::string, size_t> tag_cloud;

    public:
      classifier() :
        is_trivial(never()), num_trivial(0), taggers(), tag_cloud() {}

      classifier& trivial(const pred_t& pred) {
        is_trivial = pred;
        return *this;
      }

      classifier& classify(const tagger_t& tagger) {
        taggers.push_back(tagger);
        return *this;
      }

      classifier& classify(const pred_t& pred, const std::string& label) {
        taggers.push_back(
            [=] (const Args&... args) {
              return (pred(args...)) ? label : "";
            });
        return *this;
      }

      void check(const std::tuple<Args...>& args) {
        if (apply(is_trivial, args)) ++num_trivial;

        std::string tags;
        for (tagger_t& tagger : taggers) {
          std::string tag = apply(tagger, args);
          if (tag.empty()) continue;
          if (!tags.empty()) tags += ", ";
          tags += tag;
        }
        if (!tags.empty()) ++tag_cloud[tags];
      }

      size_t trivial() const { return num_trivial; }

      distribution distro() const {
        return distribution(tag_cloud.begin(), tag_cloud.end());
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
      const typename classifier<Args...>::tagger_t& tagger,
      classifier<Args...>&& cls)
  {
    cls.classify(tagger);
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

