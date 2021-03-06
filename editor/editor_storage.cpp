#include "editor/editor_storage.hpp"

#include "platform/platform.hpp"

#include "coding/internal/file_data.hpp"

#include "base/logging.hpp"

#include "std/string.hpp"

using namespace pugi;

namespace
{
char const * kEditorXMLFileName = "edits.xml";

string GetEditorFilePath() { return GetPlatform().WritablePathForFile(kEditorXMLFileName); }
}  // namespace

namespace editor
{
// StorageLocal ------------------------------------------------------------------------------------
bool LocalStorage::Save(xml_document const & doc)
{
  auto const editorFilePath = GetEditorFilePath();
  return my::WriteToTempAndRenameToFile(editorFilePath, [&doc](string const & fileName) {
    return doc.save_file(fileName.data(), "  " /* indent */);
  });
}

bool LocalStorage::Load(xml_document & doc)
{
  auto const editorFilePath = GetEditorFilePath();
  auto const result = doc.load_file(editorFilePath.c_str());
  // Note: status_file_not_found is ok if a user has never made any edits.
  if (result != status_ok && result != status_file_not_found)
  {
    LOG(LERROR, ("Can't load map edits from disk:", editorFilePath));
    return false;
  }

  return true;
}

void LocalStorage::Reset()
{
  my::DeleteFileX(GetEditorFilePath());
}

// StorageMemory -----------------------------------------------------------------------------------
bool InMemoryStorage::Save(xml_document const & doc)
{
  m_doc.reset(doc);
  return true;
}

bool InMemoryStorage::Load(xml_document & doc)
{
  doc.reset(m_doc);
  return true;
}

void InMemoryStorage::Reset()
{
  m_doc.reset();
}
}  // namespace editor
