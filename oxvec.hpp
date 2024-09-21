#ifndef OXVEC_HPP
#define OXVEC_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>
#include <queue>
#include <algorithm>

using Vector = std::vector<double>;
using VectorMap = std::unordered_map<std::string, std::shared_ptr<class VectorEntry>>;

class VectorEntry;
class SimilarityMetric;
class EuclideanDistance;
class CosineSimilarity;
class DotProductSimilarity;
class VectorIndex;
class VectorCollection;
class OxVec;

void printVector(const Vector& vec);

class VectorEntry {
public:
    VectorEntry(const std::string& id, const Vector& vector);

    const std::string& getId() const;
    const Vector& getVector() const;

private:
    std::string id_;
    Vector vector_;
};

class SimilarityMetric {
public:
    virtual ~SimilarityMetric() = default;
    virtual double computeSimilarity(const Vector& vector1, const Vector& vector2) const = 0;
};

class EuclideanDistance : public SimilarityMetric {
public:
    double computeSimilarity(const Vector& vector1, const Vector& vector2) const override;
};

class CosineSimilarity : public SimilarityMetric {
public:
    double computeSimilarity(const Vector& vector1, const Vector& vector2) const override;
};

class DotProductSimilarity : public SimilarityMetric {
public:
    double computeSimilarity(const Vector& vector1, const Vector& vector2) const override;
};

class VectorIndex {
public:
    void buildIndex(const VectorMap& vectors);

    std::vector<std::shared_ptr<VectorEntry>> search(
        const Vector& queryVector,
        const SimilarityMetric& metric,
        int topK,
        bool isDistanceMetric = false) const;

private:
    VectorMap vectors_;
};

class VectorCollection {
public:
    explicit VectorCollection(const std::string& name);

    void addVector(const std::string& id, const Vector& vector);
    void deleteVector(const std::string& id);
    void updateVector(const std::string& id, const Vector& vector);
    std::shared_ptr<VectorEntry> getVector(const std::string& id) const;

    std::vector<std::shared_ptr<VectorEntry>> query(
        const Vector& queryVector,
        const SimilarityMetric& metric,
        int topK) const;

    const VectorMap& getVectors() const;

private:
    std::string name_;
    VectorMap vectors_;
    std::shared_ptr<VectorIndex> index_;
};

class OxVec {
public:
    void createCollection(const std::string& name);
    void deleteCollection(const std::string& name);
    std::shared_ptr<VectorCollection> getCollection(const std::string& name) const;

private:
    std::unordered_map<std::string, std::shared_ptr<VectorCollection>> collections_;
};

#endif
