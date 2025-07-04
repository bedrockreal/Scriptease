#ifndef TAS_EDITOR_HPP
#define TAS_EDITOR_HPP
#include "script.hpp"

#include "imgui.h"

#include <string>
#include <vector>

#include <iostream>

namespace tas
{
    namespace editor
    {
        struct inputSeqWithSelection : public script::inputSeq
        {
            ImGuiSelectionBasicStorage selection;
            ImVector<ImGuiID> items_id;
            static ImVec2 context_popup_joy_pos;

            inputSeqWithSelection() : script::inputSeq()
            {
                // selection.UserData = (void*)&items_id;
                // selection.AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self, int idx)
                // {
                //     ImVector<ImGuiID>* p_items = (ImVector<ImGuiID>*)self->UserData;
                //     // std::cout << std::endl << p_items->Size << std::endl;
                //     std::cout << "p: " << (*p_items)[idx] << std::endl;
                //     return (*p_items)[idx];
                // }; // Index -> ID
                appendLines(10);
            }
            
            bool isSelected(int i);
            std::vector<int> getSelectedIdx();
            void loadFromFile(std::string filename = "") override;
            void appendLines(int cnt = 1) override;
            void insertSelected();
            void deleteSelected();
            void duplicateSelected();
            void swapSelectedUp();
            void swapSelectedDown();
            void syncID();

            private:
            static int _id;
            void insertID(std::vector<int> idx);
            void deleteID(std::vector<int> idx);
            void duplicateID(std::vector<int> idx);
            // void swapIDUp(std::vector<int> idx);
            // void swapIDDown(std::vector<int> idx);
        };

        extern const int NUM_OF_COLS;
        extern const char* const head[];
        extern std::string editor_file_name;
        extern inputSeqWithSelection loaded_input_seq;
        constexpr static int MAX_JOY_COORD_ABS = 0x7FFF;

        // struct loadedFile
        // {
        //     std::string name;
        // };

        void mainLoop();
        void saveFileNoArgs();
        void openFileNoArgs();
        void saveFile(std::string filename = "");
        void openFile(std::string filename = "");
    } // namespace editor
    
} // namespace tas


#endif