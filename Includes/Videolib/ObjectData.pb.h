// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ObjectData.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_ObjectData_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_ObjectData_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012004 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_ObjectData_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_ObjectData_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_ObjectData_2eproto;
class ObjectDataProto;
class ObjectDataProtoDefaultTypeInternal;
extern ObjectDataProtoDefaultTypeInternal _ObjectDataProto_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::ObjectDataProto* Arena::CreateMaybeMessage<::ObjectDataProto>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class ObjectDataProto PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:ObjectDataProto) */ {
 public:
  inline ObjectDataProto() : ObjectDataProto(nullptr) {};
  virtual ~ObjectDataProto();

  ObjectDataProto(const ObjectDataProto& from);
  ObjectDataProto(ObjectDataProto&& from) noexcept
    : ObjectDataProto() {
    *this = ::std::move(from);
  }

  inline ObjectDataProto& operator=(const ObjectDataProto& from) {
    CopyFrom(from);
    return *this;
  }
  inline ObjectDataProto& operator=(ObjectDataProto&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const ObjectDataProto& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ObjectDataProto* internal_default_instance() {
    return reinterpret_cast<const ObjectDataProto*>(
               &_ObjectDataProto_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ObjectDataProto& a, ObjectDataProto& b) {
    a.Swap(&b);
  }
  inline void Swap(ObjectDataProto* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ObjectDataProto* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ObjectDataProto* New() const final {
    return CreateMaybeMessage<ObjectDataProto>(nullptr);
  }

  ObjectDataProto* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ObjectDataProto>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ObjectDataProto& from);
  void MergeFrom(const ObjectDataProto& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ObjectDataProto* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "ObjectDataProto";
  }
  protected:
  explicit ObjectDataProto(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_ObjectData_2eproto);
    return ::descriptor_table_ObjectData_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kObjectFieldNumber = 1,
    kCountFieldNumber = 2,
    kTimeFieldNumber = 3,
  };
  // string object = 1;
  void clear_object();
  const std::string& object() const;
  void set_object(const std::string& value);
  void set_object(std::string&& value);
  void set_object(const char* value);
  void set_object(const char* value, size_t size);
  std::string* mutable_object();
  std::string* release_object();
  void set_allocated_object(std::string* object);
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  std::string* unsafe_arena_release_object();
  GOOGLE_PROTOBUF_RUNTIME_DEPRECATED("The unsafe_arena_ accessors for"
  "    string fields are deprecated and will be removed in a"
  "    future release.")
  void unsafe_arena_set_allocated_object(
      std::string* object);
  private:
  const std::string& _internal_object() const;
  void _internal_set_object(const std::string& value);
  std::string* _internal_mutable_object();
  public:

  // int32 count = 2;
  void clear_count();
  ::PROTOBUF_NAMESPACE_ID::int32 count() const;
  void set_count(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_count() const;
  void _internal_set_count(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // float time = 3;
  void clear_time();
  float time() const;
  void set_time(float value);
  private:
  float _internal_time() const;
  void _internal_set_time(float value);
  public:

  // @@protoc_insertion_point(class_scope:ObjectDataProto)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr object_;
  ::PROTOBUF_NAMESPACE_ID::int32 count_;
  float time_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_ObjectData_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ObjectDataProto

// string object = 1;
inline void ObjectDataProto::clear_object() {
  object_.ClearToEmpty(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline const std::string& ObjectDataProto::object() const {
  // @@protoc_insertion_point(field_get:ObjectDataProto.object)
  return _internal_object();
}
inline void ObjectDataProto::set_object(const std::string& value) {
  _internal_set_object(value);
  // @@protoc_insertion_point(field_set:ObjectDataProto.object)
}
inline std::string* ObjectDataProto::mutable_object() {
  // @@protoc_insertion_point(field_mutable:ObjectDataProto.object)
  return _internal_mutable_object();
}
inline const std::string& ObjectDataProto::_internal_object() const {
  return object_.Get();
}
inline void ObjectDataProto::_internal_set_object(const std::string& value) {
  
  object_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value, GetArena());
}
inline void ObjectDataProto::set_object(std::string&& value) {
  
  object_.Set(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:ObjectDataProto.object)
}
inline void ObjectDataProto::set_object(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  object_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value),
              GetArena());
  // @@protoc_insertion_point(field_set_char:ObjectDataProto.object)
}
inline void ObjectDataProto::set_object(const char* value,
    size_t size) {
  
  object_.Set(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:ObjectDataProto.object)
}
inline std::string* ObjectDataProto::_internal_mutable_object() {
  
  return object_.Mutable(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline std::string* ObjectDataProto::release_object() {
  // @@protoc_insertion_point(field_release:ObjectDataProto.object)
  return object_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void ObjectDataProto::set_allocated_object(std::string* object) {
  if (object != nullptr) {
    
  } else {
    
  }
  object_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), object,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:ObjectDataProto.object)
}
inline std::string* ObjectDataProto::unsafe_arena_release_object() {
  // @@protoc_insertion_point(field_unsafe_arena_release:ObjectDataProto.object)
  GOOGLE_DCHECK(GetArena() != nullptr);
  
  return object_.UnsafeArenaRelease(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      GetArena());
}
inline void ObjectDataProto::unsafe_arena_set_allocated_object(
    std::string* object) {
  GOOGLE_DCHECK(GetArena() != nullptr);
  if (object != nullptr) {
    
  } else {
    
  }
  object_.UnsafeArenaSetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      object, GetArena());
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:ObjectDataProto.object)
}

// int32 count = 2;
inline void ObjectDataProto::clear_count() {
  count_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ObjectDataProto::_internal_count() const {
  return count_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ObjectDataProto::count() const {
  // @@protoc_insertion_point(field_get:ObjectDataProto.count)
  return _internal_count();
}
inline void ObjectDataProto::_internal_set_count(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  count_ = value;
}
inline void ObjectDataProto::set_count(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_count(value);
  // @@protoc_insertion_point(field_set:ObjectDataProto.count)
}

// float time = 3;
inline void ObjectDataProto::clear_time() {
  time_ = 0;
}
inline float ObjectDataProto::_internal_time() const {
  return time_;
}
inline float ObjectDataProto::time() const {
  // @@protoc_insertion_point(field_get:ObjectDataProto.time)
  return _internal_time();
}
inline void ObjectDataProto::_internal_set_time(float value) {
  
  time_ = value;
}
inline void ObjectDataProto::set_time(float value) {
  _internal_set_time(value);
  // @@protoc_insertion_point(field_set:ObjectDataProto.time)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_ObjectData_2eproto
