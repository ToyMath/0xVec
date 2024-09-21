#include "oxvec.hpp"

void printVector(const Vector& vec) {
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        printf("%.2f", vec[i]);
        if (i != vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]";
}

VectorEntry::VectorEntry(const std::string& id, const Vector& vector)
    : id_(id), vector_(vector) {}

const std::string& VectorEntry::getId() const {
    return id_;
}

const Vector& VectorEntry::getVector() const {
    return vector_;
}

double EuclideanDistance::computeSimilarity(const Vector& vector1, const Vector& vector2) const {
    double sum = 0.0;
    for (size_t i = 0; i < vector1.size(); ++i) {
        double diff = vector1[i] - vector2[i];
        sum += diff * diff;
    }
    return std::sqrt(sum);
}

double CosineSimilarity::computeSimilarity(const Vector& vector1, const Vector& vector2) const {
    double dotProduct = 0.0;
    double magnitude1 = 0.0;
    double magnitude2 = 0.0;

    for (size_t i = 0; i < vector1.size(); ++i) {
        dotProduct += vector1[i] * vector2[i];
        magnitude1 += vector1[i] * vector1[i];
        magnitude2 += vector2[i] * vector2[i];
    }

    if (magnitude1 == 0.0 || magnitude2 == 0.0) {
        return 0.0;
    }
    return dotProduct / (std::sqrt(magnitude1) * std::sqrt(magnitude2));
}

double DotProductSimilarity::computeSimilarity(const Vector& vector1, const Vector& vector2) const {
    double dotProduct = 0.0;
    for (size_t i = 0; i < vector1.size(); ++i) {
        dotProduct += vector1[i] * vector2[i];
    }
    return dotProduct;
}

void VectorIndex::buildIndex(const VectorMap& vectors) {
    vectors_ = vectors;
}

std::vector<std::shared_ptr<VectorEntry>> VectorIndex::search(
    const Vector& queryVector,
    const SimilarityMetric& metric,
    int topK,
    bool isDistanceMetric) const
{
    using QueueElement = std::pair<double, std::shared_ptr<VectorEntry>>;
    auto compare = isDistanceMetric
        ? [](const QueueElement& a, const QueueElement& b) { return a.first < b.first; }
        : [](const QueueElement& a, const QueueElement& b) { return a.first > b.first; };

    std::priority_queue<
        QueueElement,
        std::vector<QueueElement>,
        decltype(compare)> heap(compare);

    for (const auto& pair : vectors_) {
        const auto& vectorEntry = pair.second;
        double similarity = metric.computeSimilarity(queryVector, vectorEntry->getVector());
        heap.push({ similarity, vectorEntry });
        if (heap.size() > static_cast<size_t>(topK)) {
            heap.pop();
        }
    }

    std::vector<std::shared_ptr<VectorEntry>> results;
    while (!heap.empty()) {
        results.push_back(heap.top().second);
        heap.pop();
    }

    std::reverse(results.begin(), results.end());
    return results;
}

VectorCollection::VectorCollection(const std::string& name)
    : name_(name), index_(std::make_shared<VectorIndex>()) {}

void VectorCollection::addVector(const std::string& id, const Vector& vector) {
    vectors_[id] = std::make_shared<VectorEntry>(id, vector);
    index_->buildIndex(vectors_);
}

void VectorCollection::deleteVector(const std::string& id) {
    auto it = vectors_.find(id);
    if (it != vectors_.end()) {
        vectors_.erase(it);
        index_->buildIndex(vectors_);
    }
}

void VectorCollection::updateVector(const std::string& id, const Vector& vector) {
    vectors_[id] = std::make_shared<VectorEntry>(id, vector);
    index_->buildIndex(vectors_);
}

std::shared_ptr<VectorEntry> VectorCollection::getVector(const std::string& id) const {
    auto it = vectors_.find(id);
    if (it != vectors_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<VectorEntry>> VectorCollection::query(
    const Vector& queryVector,
    const SimilarityMetric& metric,
    int topK) const
{
    bool isDistanceMetric = dynamic_cast<const EuclideanDistance*>(&metric) != nullptr;
    return index_->search(queryVector, metric, topK, isDistanceMetric);
}

const VectorMap& VectorCollection::getVectors() const {
    return vectors_;
}

void OxVec::createCollection(const std::string& name) {
    if (collections_.find(name) == collections_.end()) {
        collections_[name] = std::make_shared<VectorCollection>(name);
    }
}

void OxVec::deleteCollection(const std::string& name) {
    auto it = collections_.find(name);
    if (it != collections_.end()) {
        collections_.erase(it);
    }
}

std::shared_ptr<VectorCollection> OxVec::getCollection(const std::string& name) const {
    auto it = collections_.find(name);
    if (it != collections_.end()) {
        return it->second;
    }
    return nullptr;
}
