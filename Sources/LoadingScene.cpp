#include "Core.h"

void LoadingScene::Init()
{
	curIndex = 0;
	assetFiles = GetAssetFiles("Assets");
	ldSprite.LoadAll(L"Assets/UI/Loading/");
	ldProperty.pos = D3DXVECTOR2(1280 / 2, 720 / 2);
}

void LoadingScene::Term()
{
}

void LoadingScene::Update(float deltaTime)
{
	ldSprite.Update(deltaTime);
    if (curIndex >= assetFiles.size())
    {
        SceneManager::GetInstance().AddScene(L"Main", std::shared_ptr<Scene>(new MainScene));
        SceneManager::GetInstance().ChangeScene(L"Main");
        return;
    }
    wstring path;
    path.assign(assetFiles[curIndex].begin(), assetFiles[curIndex].end());
    TextureManager::GetInstance().GetTexture(path);
    ++curIndex;
}

void LoadingScene::Render()
{
    ldSprite.Render(ldProperty);
}

vector<string> LoadingScene::GetAssetFiles(string path)
{
	vector<string> result;

    char findPath[255];
    sprintf(findPath, "%s/*.*", path.c_str());

    _finddata_t fd;
    intptr_t handle;

    if ((handle = _findfirst(findPath, &fd)) != -1L)
    {
        do
        {
            char buffer[255];
            sprintf(buffer, "%s/%s", path.c_str(), fd.name);
            if (fd.attrib & _A_SUBDIR && fd.name[0] != '.')
            {
                vector<string> returnVector = GetAssetFiles(buffer);
                result.insert(result.end(), returnVector.begin(), returnVector.end());
            }
            else if (fd.size != 0 && fd.name[0] != '.')
            {
                result.push_back(buffer);
            }
        } while (_findnext(handle, &fd) == 0);
        _findclose(handle);
    }

	return result;
}
