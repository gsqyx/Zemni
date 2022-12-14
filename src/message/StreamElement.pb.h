// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: StreamElement.proto

#ifndef PROTOBUF_INCLUDED_StreamElement_2eproto
#define PROTOBUF_INCLUDED_StreamElement_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_StreamElement_2eproto 

namespace protobuf_StreamElement_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[2];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_StreamElement_2eproto
namespace Zemni_protobuf {
class StreamElement;
class StreamElementDefaultTypeInternal;
extern StreamElementDefaultTypeInternal _StreamElement_default_instance_;
class StreamElements;
class StreamElementsDefaultTypeInternal;
extern StreamElementsDefaultTypeInternal _StreamElements_default_instance_;
}  // namespace Zemni_protobuf
namespace google {
namespace protobuf {
template<> ::Zemni_protobuf::StreamElement* Arena::CreateMaybeMessage<::Zemni_protobuf::StreamElement>(Arena*);
template<> ::Zemni_protobuf::StreamElements* Arena::CreateMaybeMessage<::Zemni_protobuf::StreamElements>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace Zemni_protobuf {

enum StreamElementType {
  STREAM_ELEMENT_TYPE_UNKNOWN = 0,
  STREAM_ELEMENT_TYPE_RECORD = 1,
  STREAM_ELEMENT_TYPE_WATERMARK = 2,
  STREAM_ELEMENT_TYPE_SIGNAL_EOF = 3,
  STREAM_ELEMENT_TYPE_OPERATOR_STOP_SIGNAL = 4,
  STREAM_ELEMENT_TYPE_CHECKPOINT_BARRIER = 5,
  STREAM_ELEMENT_TYPE_TIMESTAMP = 6,
  StreamElementType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  StreamElementType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool StreamElementType_IsValid(int value);
const StreamElementType StreamElementType_MIN = STREAM_ELEMENT_TYPE_UNKNOWN;
const StreamElementType StreamElementType_MAX = STREAM_ELEMENT_TYPE_TIMESTAMP;
const int StreamElementType_ARRAYSIZE = StreamElementType_MAX + 1;

const ::google::protobuf::EnumDescriptor* StreamElementType_descriptor();
inline const ::std::string& StreamElementType_Name(StreamElementType value) {
  return ::google::protobuf::internal::NameOfEnum(
    StreamElementType_descriptor(), value);
}
inline bool StreamElementType_Parse(
    const ::std::string& name, StreamElementType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<StreamElementType>(
    StreamElementType_descriptor(), name, value);
}
// ===================================================================

class StreamElement : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Zemni_protobuf.StreamElement) */ {
 public:
  StreamElement();
  virtual ~StreamElement();

  StreamElement(const StreamElement& from);

  inline StreamElement& operator=(const StreamElement& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  StreamElement(StreamElement&& from) noexcept
    : StreamElement() {
    *this = ::std::move(from);
  }

  inline StreamElement& operator=(StreamElement&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const StreamElement& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const StreamElement* internal_default_instance() {
    return reinterpret_cast<const StreamElement*>(
               &_StreamElement_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(StreamElement* other);
  friend void swap(StreamElement& a, StreamElement& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline StreamElement* New() const final {
    return CreateMaybeMessage<StreamElement>(NULL);
  }

  StreamElement* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<StreamElement>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const StreamElement& from);
  void MergeFrom(const StreamElement& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(StreamElement* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // bytes key = 4;
  void clear_key();
  static const int kKeyFieldNumber = 4;
  const ::std::string& key() const;
  void set_key(const ::std::string& value);
  #if LANG_CXX11
  void set_key(::std::string&& value);
  #endif
  void set_key(const char* value);
  void set_key(const void* value, size_t size);
  ::std::string* mutable_key();
  ::std::string* release_key();
  void set_allocated_key(::std::string* key);

  // bytes value1 = 5;
  void clear_value1();
  static const int kValue1FieldNumber = 5;
  const ::std::string& value1() const;
  void set_value1(const ::std::string& value);
  #if LANG_CXX11
  void set_value1(::std::string&& value);
  #endif
  void set_value1(const char* value);
  void set_value1(const void* value, size_t size);
  ::std::string* mutable_value1();
  ::std::string* release_value1();
  void set_allocated_value1(::std::string* value1);

  // bytes value2 = 6;
  void clear_value2();
  static const int kValue2FieldNumber = 6;
  const ::std::string& value2() const;
  void set_value2(const ::std::string& value);
  #if LANG_CXX11
  void set_value2(::std::string&& value);
  #endif
  void set_value2(const char* value);
  void set_value2(const void* value, size_t size);
  ::std::string* mutable_value2();
  ::std::string* release_value2();
  void set_allocated_value2(::std::string* value2);

  // int64 id = 2;
  void clear_id();
  static const int kIdFieldNumber = 2;
  ::google::protobuf::int64 id() const;
  void set_id(::google::protobuf::int64 value);

  // int64 timestamp = 3;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 3;
  ::google::protobuf::int64 timestamp() const;
  void set_timestamp(::google::protobuf::int64 value);

  // int32 type = 1;
  void clear_type();
  static const int kTypeFieldNumber = 1;
  ::google::protobuf::int32 type() const;
  void set_type(::google::protobuf::int32 value);

  // int32 int_value = 7;
  void clear_int_value();
  static const int kIntValueFieldNumber = 7;
  ::google::protobuf::int32 int_value() const;
  void set_int_value(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Zemni_protobuf.StreamElement)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr key_;
  ::google::protobuf::internal::ArenaStringPtr value1_;
  ::google::protobuf::internal::ArenaStringPtr value2_;
  ::google::protobuf::int64 id_;
  ::google::protobuf::int64 timestamp_;
  ::google::protobuf::int32 type_;
  ::google::protobuf::int32 int_value_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_StreamElement_2eproto::TableStruct;
};
// -------------------------------------------------------------------

class StreamElements : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:Zemni_protobuf.StreamElements) */ {
 public:
  StreamElements();
  virtual ~StreamElements();

  StreamElements(const StreamElements& from);

  inline StreamElements& operator=(const StreamElements& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  StreamElements(StreamElements&& from) noexcept
    : StreamElements() {
    *this = ::std::move(from);
  }

  inline StreamElements& operator=(StreamElements&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const StreamElements& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const StreamElements* internal_default_instance() {
    return reinterpret_cast<const StreamElements*>(
               &_StreamElements_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  void Swap(StreamElements* other);
  friend void swap(StreamElements& a, StreamElements& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline StreamElements* New() const final {
    return CreateMaybeMessage<StreamElements>(NULL);
  }

  StreamElements* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<StreamElements>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const StreamElements& from);
  void MergeFrom(const StreamElements& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(StreamElements* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .Zemni_protobuf.StreamElement element = 1;
  int element_size() const;
  void clear_element();
  static const int kElementFieldNumber = 1;
  ::Zemni_protobuf::StreamElement* mutable_element(int index);
  ::google::protobuf::RepeatedPtrField< ::Zemni_protobuf::StreamElement >*
      mutable_element();
  const ::Zemni_protobuf::StreamElement& element(int index) const;
  ::Zemni_protobuf::StreamElement* add_element();
  const ::google::protobuf::RepeatedPtrField< ::Zemni_protobuf::StreamElement >&
      element() const;

  // int32 queue_id = 2;
  void clear_queue_id();
  static const int kQueueIdFieldNumber = 2;
  ::google::protobuf::int32 queue_id() const;
  void set_queue_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Zemni_protobuf.StreamElements)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::RepeatedPtrField< ::Zemni_protobuf::StreamElement > element_;
  ::google::protobuf::int32 queue_id_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_StreamElement_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// StreamElement

// int32 type = 1;
inline void StreamElement::clear_type() {
  type_ = 0;
}
inline ::google::protobuf::int32 StreamElement::type() const {
  // @@protoc_insertion_point(field_get:Zemni_protobuf.StreamElement.type)
  return type_;
}
inline void StreamElement::set_type(::google::protobuf::int32 value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:Zemni_protobuf.StreamElement.type)
}

// int64 id = 2;
inline void StreamElement::clear_id() {
  id_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 StreamElement::id() const {
  // @@protoc_insertion_point(field_get:Zemni_protobuf.StreamElement.id)
  return id_;
}
inline void StreamElement::set_id(::google::protobuf::int64 value) {
  
  id_ = value;
  // @@protoc_insertion_point(field_set:Zemni_protobuf.StreamElement.id)
}

// int64 timestamp = 3;
inline void StreamElement::clear_timestamp() {
  timestamp_ = GOOGLE_LONGLONG(0);
}
inline ::google::protobuf::int64 StreamElement::timestamp() const {
  // @@protoc_insertion_point(field_get:Zemni_protobuf.StreamElement.timestamp)
  return timestamp_;
}
inline void StreamElement::set_timestamp(::google::protobuf::int64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:Zemni_protobuf.StreamElement.timestamp)
}

// bytes key = 4;
inline void StreamElement::clear_key() {
  key_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& StreamElement::key() const {
  // @@protoc_insertion_point(field_get:Zemni_protobuf.StreamElement.key)
  return key_.GetNoArena();
}
inline void StreamElement::set_key(const ::std::string& value) {
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Zemni_protobuf.StreamElement.key)
}
#if LANG_CXX11
inline void StreamElement::set_key(::std::string&& value) {
  
  key_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Zemni_protobuf.StreamElement.key)
}
#endif
inline void StreamElement::set_key(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Zemni_protobuf.StreamElement.key)
}
inline void StreamElement::set_key(const void* value, size_t size) {
  
  key_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Zemni_protobuf.StreamElement.key)
}
inline ::std::string* StreamElement::mutable_key() {
  
  // @@protoc_insertion_point(field_mutable:Zemni_protobuf.StreamElement.key)
  return key_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* StreamElement::release_key() {
  // @@protoc_insertion_point(field_release:Zemni_protobuf.StreamElement.key)
  
  return key_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void StreamElement::set_allocated_key(::std::string* key) {
  if (key != NULL) {
    
  } else {
    
  }
  key_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), key);
  // @@protoc_insertion_point(field_set_allocated:Zemni_protobuf.StreamElement.key)
}

// bytes value1 = 5;
inline void StreamElement::clear_value1() {
  value1_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& StreamElement::value1() const {
  // @@protoc_insertion_point(field_get:Zemni_protobuf.StreamElement.value1)
  return value1_.GetNoArena();
}
inline void StreamElement::set_value1(const ::std::string& value) {
  
  value1_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Zemni_protobuf.StreamElement.value1)
}
#if LANG_CXX11
inline void StreamElement::set_value1(::std::string&& value) {
  
  value1_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Zemni_protobuf.StreamElement.value1)
}
#endif
inline void StreamElement::set_value1(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  value1_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Zemni_protobuf.StreamElement.value1)
}
inline void StreamElement::set_value1(const void* value, size_t size) {
  
  value1_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Zemni_protobuf.StreamElement.value1)
}
inline ::std::string* StreamElement::mutable_value1() {
  
  // @@protoc_insertion_point(field_mutable:Zemni_protobuf.StreamElement.value1)
  return value1_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* StreamElement::release_value1() {
  // @@protoc_insertion_point(field_release:Zemni_protobuf.StreamElement.value1)
  
  return value1_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void StreamElement::set_allocated_value1(::std::string* value1) {
  if (value1 != NULL) {
    
  } else {
    
  }
  value1_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value1);
  // @@protoc_insertion_point(field_set_allocated:Zemni_protobuf.StreamElement.value1)
}

// bytes value2 = 6;
inline void StreamElement::clear_value2() {
  value2_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& StreamElement::value2() const {
  // @@protoc_insertion_point(field_get:Zemni_protobuf.StreamElement.value2)
  return value2_.GetNoArena();
}
inline void StreamElement::set_value2(const ::std::string& value) {
  
  value2_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:Zemni_protobuf.StreamElement.value2)
}
#if LANG_CXX11
inline void StreamElement::set_value2(::std::string&& value) {
  
  value2_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:Zemni_protobuf.StreamElement.value2)
}
#endif
inline void StreamElement::set_value2(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  value2_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:Zemni_protobuf.StreamElement.value2)
}
inline void StreamElement::set_value2(const void* value, size_t size) {
  
  value2_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:Zemni_protobuf.StreamElement.value2)
}
inline ::std::string* StreamElement::mutable_value2() {
  
  // @@protoc_insertion_point(field_mutable:Zemni_protobuf.StreamElement.value2)
  return value2_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* StreamElement::release_value2() {
  // @@protoc_insertion_point(field_release:Zemni_protobuf.StreamElement.value2)
  
  return value2_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void StreamElement::set_allocated_value2(::std::string* value2) {
  if (value2 != NULL) {
    
  } else {
    
  }
  value2_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value2);
  // @@protoc_insertion_point(field_set_allocated:Zemni_protobuf.StreamElement.value2)
}

// int32 int_value = 7;
inline void StreamElement::clear_int_value() {
  int_value_ = 0;
}
inline ::google::protobuf::int32 StreamElement::int_value() const {
  // @@protoc_insertion_point(field_get:Zemni_protobuf.StreamElement.int_value)
  return int_value_;
}
inline void StreamElement::set_int_value(::google::protobuf::int32 value) {
  
  int_value_ = value;
  // @@protoc_insertion_point(field_set:Zemni_protobuf.StreamElement.int_value)
}

// -------------------------------------------------------------------

// StreamElements

// repeated .Zemni_protobuf.StreamElement element = 1;
inline int StreamElements::element_size() const {
  return element_.size();
}
inline void StreamElements::clear_element() {
  element_.Clear();
}
inline ::Zemni_protobuf::StreamElement* StreamElements::mutable_element(int index) {
  // @@protoc_insertion_point(field_mutable:Zemni_protobuf.StreamElements.element)
  return element_.Mutable(index);
}
inline ::google::protobuf::RepeatedPtrField< ::Zemni_protobuf::StreamElement >*
StreamElements::mutable_element() {
  // @@protoc_insertion_point(field_mutable_list:Zemni_protobuf.StreamElements.element)
  return &element_;
}
inline const ::Zemni_protobuf::StreamElement& StreamElements::element(int index) const {
  // @@protoc_insertion_point(field_get:Zemni_protobuf.StreamElements.element)
  return element_.Get(index);
}
inline ::Zemni_protobuf::StreamElement* StreamElements::add_element() {
  // @@protoc_insertion_point(field_add:Zemni_protobuf.StreamElements.element)
  return element_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::Zemni_protobuf::StreamElement >&
StreamElements::element() const {
  // @@protoc_insertion_point(field_list:Zemni_protobuf.StreamElements.element)
  return element_;
}

// int32 queue_id = 2;
inline void StreamElements::clear_queue_id() {
  queue_id_ = 0;
}
inline ::google::protobuf::int32 StreamElements::queue_id() const {
  // @@protoc_insertion_point(field_get:Zemni_protobuf.StreamElements.queue_id)
  return queue_id_;
}
inline void StreamElements::set_queue_id(::google::protobuf::int32 value) {
  
  queue_id_ = value;
  // @@protoc_insertion_point(field_set:Zemni_protobuf.StreamElements.queue_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace Zemni_protobuf

namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::Zemni_protobuf::StreamElementType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Zemni_protobuf::StreamElementType>() {
  return ::Zemni_protobuf::StreamElementType_descriptor();
}

}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_StreamElement_2eproto
