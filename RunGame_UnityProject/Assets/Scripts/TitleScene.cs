using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;

// タイトル画面の進行を制御します。
public class TitleScene : MonoBehaviour
{
    // 次のシーンを読み込み可能な場合はtrue、それ以外はfalse
    bool isLoadable = false;

    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(OnStart());
    }

    // ２秒待機後に次のシーンを読込み可能にします。
    IEnumerator OnStart()
    {
        yield return new WaitForSeconds(2);
        isLoadable = true;
    }

    // 決定ボタン操作の際に呼び出され、次のシーンを読み込みます。
    public void LoadNextScene()
    {
        if (isLoadable)
        {
            SceneManager.LoadScene("Stage 0");
        }
    }

    // ゲームを終了します。
    public void ExitGame()
    {
        Application.Quit();
    }
}
