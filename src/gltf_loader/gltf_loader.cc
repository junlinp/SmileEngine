
#include <_types/_uint16_t.h>
#include <iostream>
#include <memory>
#include <fstream>
#include "libgltf/libgltf.h"
#include <algorithm>
#include <string>
#include "gltf_loader.h"


namespace base64 {

inline std::string get_base64_chars() {
    static std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "0123456789+/";
    return base64_chars;
}

inline std::string to_base64(std::string const &data) {
  int counter = 0;
  uint32_t bit_stream = 0;
  const std::string base64_chars = get_base64_chars();
  std::string encoded;
  int offset = 0;
  for (unsigned char c : data) {
    auto num_val = static_cast<unsigned int>(c);
    offset = 16 - counter % 3 * 8;
    bit_stream += num_val << offset;
    if (offset == 16) {
      encoded += base64_chars.at(bit_stream >> 18 & 0x3f);
    }
    if (offset == 8) {
      encoded += base64_chars.at(bit_stream >> 12 & 0x3f);
    }
    if (offset == 0 && counter != 3) {
      encoded += base64_chars.at(bit_stream >> 6 & 0x3f);
      encoded += base64_chars.at(bit_stream & 0x3f);
      bit_stream = 0;
    }
    counter++;
  }
  if (offset == 16) {
    encoded += base64_chars.at(bit_stream >> 12 & 0x3f);
    encoded += "==";
  }
  if (offset == 8) {
    encoded += base64_chars.at(bit_stream >> 6 & 0x3f);
    encoded += '=';
  }
  return encoded;
}

inline std::string from_base64(std::string const &data) {
  int counter = 0;
  uint32_t bit_stream = 0;
  std::string decoded;
  int offset = 0;
  const std::string base64_chars = get_base64_chars();
  for (unsigned char c : data) {
    auto num_val = base64_chars.find(c);
    if (num_val != std::string::npos) {
      offset = 18 - counter % 4 * 6;
      bit_stream += num_val << offset;
      if (offset == 12) {
        decoded += static_cast<char>(bit_stream >> 16 & 0xff);
      }
      if (offset == 6) {
        decoded += static_cast<char>(bit_stream >> 8 & 0xff);
      }
      if (offset == 0 && counter != 4) {
        decoded += static_cast<char>(bit_stream & 0xff);
        bit_stream = 0;
      }
    } else if (c != '=') {
      return std::string();
    }
    counter++;
  }
  return decoded;
}

}

std::vector<Eigen::Vector3f> LoadVertex(const char* buffer, size_t length) {
    int component_size = sizeof(float) * 3;

    assert(length % component_size == 0);

    std::vector<Eigen::Vector3f> res;

    float* float_ptr = (float*)buffer;
    for(int i = 0; i < (length / component_size); i++) {
        Eigen::Map<const Eigen::Vector3f> mapping(float_ptr + i * 3);
        res.push_back(mapping);
    }
    return res;
}

std::vector<uint16_t> LoadIndex(const char* buffer, size_t length) {
    std::vector<uint16_t> indices;
    assert(length % sizeof(uint16_t) == 0);

    uint16_t* ptr = (uint16_t*)buffer;

    for(int i = 0; i < length / sizeof(uint16_t); i++) {
        indices.push_back(ptr[i]);
    }
    return indices;
}


std::string BufferRawData(std::shared_ptr<libgltf::SBuffer> buffer) {
  std::string uri = buffer->uri;
  std::string buffer_data = base64::from_base64(uri.substr(uri.find(",") + 1));
  return buffer_data;
}

void LoadGLTF(const std::string& path, std::vector<Mesh>* meshs) {
  std::shared_ptr<libgltf::IglTFLoader> gltf_loader =
      libgltf::IglTFLoader::Create([path](const std::string &p) {
        return std::make_shared<std::ifstream>(path);
      });
  std::cout << "create" << std::endl;
  const std::unique_ptr<libgltf::SGlTF> &loaded_gltf =
      std::move(gltf_loader->glTF());
  if (!loaded_gltf) {
    printf("failed to load your gltf file");
  }
  // get all indices of the triangle
  //libgltf::TVertexList<1, size_t> triangle_data;
  //std::shared_ptr<libgltf::TAccessorStream<libgltf::TVertexList<1, size_t>>>
      //triangle_stream = std::make_shared<
          //libgltf::TAccessorStream<libgltf::TVertexList<1, size_t>>>(
          //triangle_data);
  //gltf_loader->LoadMeshPrimitiveIndicesData(0, 0, triangle_stream);
  //gltf_loader->LoadMeshPrimitiveAttributeData(size_t _mesh_index, size_t _primitive_index, const std::string &_attribute, std::shared_ptr<IAccessorStream> _accessor_stream)
  auto accessors = loaded_gltf->accessors;

  std::vector<std::shared_ptr<libgltf::SMesh>> mesh = loaded_gltf->meshes;
  auto buffers = loaded_gltf->buffers;
  auto buffer_views = loaded_gltf->bufferViews;

   std::cout << "mesh size" << mesh.size() << std::endl;
   for (auto mesh_ptr : mesh) {
    for (auto primitive : mesh_ptr->primitives) {
        // int32_t vertex_index = primitive->indices->int32_tValue;
        std::cout << "POSITION : " << primitive->attributes.count("POSITION") << std::endl;
        if (primitive->attributes.count("POSITION") > 0) {
          int32_t vertex_index = primitive->attributes["POSITION"]->int32_tValue;
          std::shared_ptr<libgltf::SAccessor> vertex_accessors =
              accessors[vertex_index];
            int32_t buffer_view_id = vertex_accessors->bufferView->int32_tValue;
            auto vertex_buffer_view = buffer_views[buffer_view_id];
            int32_t buffer_view_offset = vertex_buffer_view->byteOffset;

            auto buffer = buffers[vertex_buffer_view->buffer->int32_tValue];
            std::string uri = buffer->uri;
            // std::cout << "uri : " << buffer->uri << std::endl;
            std::cout << "uri : " << uri.find(";") << std::endl;
            std::cout << "uri content : " << uri.substr(uri.find(","), 10)
                      << std::endl;
            std::string buffer_data =
                base64::from_base64(uri.substr(uri.find(",") + 1));
            std::cout << (buffer_data.size() == buffer->byteLength)
                      << std::endl;
            std::cout << "buffer size : " << buffer_data.size() << std::endl;
            std::cout << "buffer byteLength : " << buffer->byteLength
                      << std::endl;

            char *raw_buffer = buffer_data.data();
            std::cout << "offset : " << vertex_buffer_view->byteOffset
                      << std::endl;
            std::cout << "stride : " << vertex_buffer_view->byteStride
                      << std::endl;
            std::cout << "length: " << vertex_buffer_view->byteLength
                      << std::endl;
            auto vertex = LoadVertex(raw_buffer, vertex_buffer_view->byteLength);
            std::cout << "vertex accessors component type : "
                      << vertex_accessors->componentType << std::endl;
            std::cout << "vertex accessors component type : "
                      << vertex_accessors->type << std::endl;

            int32_t indices_index = primitive->indices->int32_tValue;
            auto indices_accessors = accessors[indices_index];
            int32_t indices_buffer_view_id = indices_accessors->bufferView->int32_tValue;
            auto indices_buffer_view = buffer_views[indices_buffer_view_id];
            int32_t indices_buffer_view_offset = indices_buffer_view->byteOffset;
            auto indices_buffer = buffers[vertex_buffer_view->buffer->int32_tValue];
            std::string indices_buffer_raw_data = BufferRawData(indices_buffer);
            auto inindes = LoadIndex(indices_buffer_raw_data.data() + indices_buffer_view_offset, indices_buffer_view->byteLength);

            std::cout << "indicesaccessors component type : "
                      << indices_accessors->componentType << std::endl;
            std::cout << "indicesaccessors component type : "
                      << indices_accessors->type << std::endl;
            std::cout << indices_accessors->count << std::endl;

            std::cout << "indexs size : " << inindes.size() << std::endl;
            std::cout << "vertex size : " << vertex.size() << std::endl;
            
            Mesh mesh;
            mesh.SetIndices(inindes);
            mesh.SetVertices(vertex);
            std::cout << "Mesh " << mesh.GetTriangles().size() << "triangles" << std::endl;
            meshs->push_back(mesh);
        }
   }
   }
}
