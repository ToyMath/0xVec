#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "oxvec.hpp"

namespace py = pybind11;

PYBIND11_MODULE(oxvec, m) {
    m.doc() = "0xVec: In-Memory Vector Database";

    py::class_<VectorEntry, std::shared_ptr<VectorEntry>>(m, "VectorEntry")
        .def(py::init<const std::string&, const Vector&>())
        .def("get_id", &VectorEntry::getId)
        .def("get_vector", &VectorEntry::getVector);

    py::class_<SimilarityMetric, std::shared_ptr<SimilarityMetric>>(m, "SimilarityMetric");

    py::class_<EuclideanDistance, SimilarityMetric, std::shared_ptr<EuclideanDistance>>(m, "EuclideanDistance")
        .def(py::init<>())
        .def("compute_similarity", &EuclideanDistance::computeSimilarity);

    py::class_<CosineSimilarity, SimilarityMetric, std::shared_ptr<CosineSimilarity>>(m, "CosineSimilarity")
        .def(py::init<>())
        .def("compute_similarity", &CosineSimilarity::computeSimilarity);

    py::class_<DotProductSimilarity, SimilarityMetric, std::shared_ptr<DotProductSimilarity>>(m, "DotProductSimilarity")
        .def(py::init<>())
        .def("compute_similarity", &DotProductSimilarity::computeSimilarity);

    py::class_<VectorCollection, std::shared_ptr<VectorCollection>>(m, "VectorCollection")
        .def(py::init<const std::string&>())
        .def("add_vector", &VectorCollection::addVector)
        .def("delete_vector", &VectorCollection::deleteVector)
        .def("update_vector", &VectorCollection::updateVector)
        .def("get_vector", &VectorCollection::getVector)
        .def("query",
            [](const VectorCollection& self,
               const Vector& queryVector,
               const SimilarityMetric& metric,
               int topK) {
                return self.query(queryVector, metric, topK);
            },
            py::arg("query_vector"),
            py::arg("metric"),
            py::arg("topK"))

        .def_property_readonly("vectors",
            [](const VectorCollection& self) {
                py::dict py_vectors;
                for (const auto& pair : self.getVectors()) {
                    py_vectors[pair.first.c_str()] = pair.second;
                }
                return py_vectors;
            });

    py::class_<OxVec, std::shared_ptr<OxVec>>(m, "OxVec")
        .def(py::init<>())
        .def("create_collection", &OxVec::createCollection)
        .def("delete_collection", &OxVec::deleteCollection)
        .def("get_collection", &OxVec::getCollection);
}
