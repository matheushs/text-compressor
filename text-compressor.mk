##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=text-compressor
ConfigurationName      :=Debug
WorkspacePath          := "/home/seduq/GitHub"
ProjectPath            := "/home/seduq/GitHub/text-compressor"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Gustavo Ceccon
Date                   :=05/14/16
CodeLitePath           :="/home/seduq/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="text-compressor.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)include/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_huffman.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bwt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_runlength.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_huffman.cpp$(ObjectSuffix): src/huffman.cpp $(IntermediateDirectory)/src_huffman.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/seduq/GitHub/text-compressor/src/huffman.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_huffman.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_huffman.cpp$(DependSuffix): src/huffman.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_huffman.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_huffman.cpp$(DependSuffix) -MM "src/huffman.cpp"

$(IntermediateDirectory)/src_huffman.cpp$(PreprocessSuffix): src/huffman.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_huffman.cpp$(PreprocessSuffix) "src/huffman.cpp"

$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/seduq/GitHub/text-compressor/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM "src/main.cpp"

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) "src/main.cpp"

$(IntermediateDirectory)/src_bwt.cpp$(ObjectSuffix): src/bwt.cpp $(IntermediateDirectory)/src_bwt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/seduq/GitHub/text-compressor/src/bwt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bwt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bwt.cpp$(DependSuffix): src/bwt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bwt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bwt.cpp$(DependSuffix) -MM "src/bwt.cpp"

$(IntermediateDirectory)/src_bwt.cpp$(PreprocessSuffix): src/bwt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bwt.cpp$(PreprocessSuffix) "src/bwt.cpp"

$(IntermediateDirectory)/src_runlength.cpp$(ObjectSuffix): src/runlength.cpp $(IntermediateDirectory)/src_runlength.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/seduq/GitHub/text-compressor/src/runlength.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_runlength.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_runlength.cpp$(DependSuffix): src/runlength.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_runlength.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_runlength.cpp$(DependSuffix) -MM "src/runlength.cpp"

$(IntermediateDirectory)/src_runlength.cpp$(PreprocessSuffix): src/runlength.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_runlength.cpp$(PreprocessSuffix) "src/runlength.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


